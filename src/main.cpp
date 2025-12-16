#include "cli.hpp"
#include "io.hpp"
#include "ll.hpp"
#include "version.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace {

struct Result {
    std::uint64_t p{};
    bool p_is_prime{};
    bool mersenne_is_prime{};
    double seconds{};
    std::string note;
};

static std::string now_ms() {
    using namespace std::chrono;
    auto t = system_clock::now().time_since_epoch();
    auto ms = duration_cast<milliseconds>(t).count();
    return std::to_string(ms);
}

} // namespace

int main(int argc, char** argv) {
    using namespace mersenne;

    CliOptions opt;
    try {
        opt = parse_cli(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "CLI error: " << e.what() << "\n\n";
        print_help(argv[0]);
        return 2;
    }

    std::vector<std::uint64_t> exps;
    try {
        if (!opt.input_file.empty()) {
            exps = read_exponents_file(opt.input_file);
        } else {
            exps = parse_exponents_from_args(opt.exps_args);
        }
    } catch (const std::exception& e) {
        std::cerr << "Input error: " << e.what() << "\n";
        return 2;
    }

    if (exps.empty()) {
        std::cerr << "No exponents provided.\n\n";
        print_help(argv[0]);
        return 2;
    }

    // de-dup + sort for nicer output
    std::sort(exps.begin(), exps.end());
    exps.erase(std::unique(exps.begin(), exps.end()), exps.end());

    std::uint32_t threads = opt.threads;
    if (threads == 0) {
        threads = std::max(1u, std::thread::hardware_concurrency());
    }

    // simple "work queue" via atomic index
    std::atomic<std::size_t> idx{0};
    std::vector<Result> results;
    results.reserve(exps.size());
    std::mutex results_mx;

    auto worker = [&]() {
        while (true) {
            std::size_t i = idx.fetch_add(1);
            if (i >= exps.size()) break;

            Result r;
            r.p = exps[i];

            auto t0 = std::chrono::steady_clock::now();

            r.p_is_prime = is_prime_u64(r.p);
            if (!r.p_is_prime && opt.skip_non_prime_p) {
                r.mersenne_is_prime = false;
                r.note = "skipped (p is composite)";
            } else {
                r.mersenne_is_prime = lucas_lehmer(r.p);
            }

            auto t1 = std::chrono::steady_clock::now();
            r.seconds = std::chrono::duration<double>(t1 - t0).count();

            {
                std::lock_guard<std::mutex> lk(results_mx);
                results.push_back(std::move(r));
            }
        }
    };

    std::vector<std::thread> pool;
    pool.reserve(threads);
    for (std::uint32_t t = 0; t < threads; ++t) pool.emplace_back(worker);
    for (auto& th : pool) th.join();

    // Keep results sorted by p
    std::sort(results.begin(), results.end(), [](const Result& a, const Result& b){ return a.p < b.p; });

    if (opt.jsonl) {
        // JSON Lines output (easy to parse)
        for (const auto& r : results) {
            std::cout
              << "{"
              << "\"p\":" << r.p << ","
              << "\"p_is_prime\":" << (r.p_is_prime ? "true" : "false") << ","
              << "\"mersenne_is_prime\":" << (r.mersenne_is_prime ? "true" : "false") << ","
              << "\"seconds\":" << std::fixed << std::setprecision(6) << r.seconds << ","
              << "\"note\":\"" << r.note << "\""
              << "}\n";
        }
        return 0;
    }

    // Pretty table output
    std::cout << "mersenne " << kVersion
              << " | threads=" << threads
              << " | items=" << results.size()
              << " | run_id=" << now_ms()
              << "\n\n";

    std::cout << std::left
              << std::setw(10) << "p"
              << std::setw(12) << "p prime?"
              << std::setw(18) << "M_p prime?"
              << std::setw(12) << "time(s)"
              << "note"
              << "\n";

    const std::string sep(10 + 12 + 18 + 12 + 20, '-');
    std::cout << sep << "\n";

    for (const auto& r : results) {
        std::cout << std::left
                  << std::setw(10) << r.p
                  << std::setw(12) << (r.p_is_prime ? "yes" : "no")
                  << std::setw(18) << (r.mersenne_is_prime ? "PRIME" : "composite")
                  << std::setw(12) << std::fixed << std::setprecision(4) << r.seconds
                  << r.note
                  << "\n";
    }

    return 0;
}
