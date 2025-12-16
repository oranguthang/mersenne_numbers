#include "cli.hpp"
#include "version.hpp"
#include <iostream>
#include <stdexcept>

namespace mersenne {

void print_help(const char* prog) {
    std::cout
      << "mersenne " << kVersion << "\n"
      << "Usage:\n"
      << "  " << prog << " --input exponents.txt [--threads N] [--jsonl]\n"
      << "  " << prog << " 31 61 89 [--threads N] [--jsonl]\n"
      << "\n"
      << "Options:\n"
      << "  --input <path>     Read exponents from file (lines: numbers, # comments allowed)\n"
      << "  --threads <N>      Worker threads (0=auto)\n"
      << "  --jsonl            Output JSON Lines (one result per line)\n"
      << "  --no-skip          Do not skip composite exponents p (default: skip)\n"
      << "  -h, --help         Show this help\n"
      << "\n"
      << "Examples:\n"
      << "  " << prog << " --input exps.txt --threads 8\n"
      << "  " << prog << " 127 521 607 --jsonl\n";
}

CliOptions parse_cli(int argc, char** argv) {
    CliOptions opt;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];

        if (a == "-h" || a == "--help") {
            print_help(argv[0]);
            std::exit(0);
        } else if (a == "--input") {
            if (i + 1 >= argc) throw std::runtime_error("--input requires a path");
            opt.input_file = argv[++i];
        } else if (a == "--threads") {
            if (i + 1 >= argc) throw std::runtime_error("--threads requires a number");
            opt.threads = static_cast<std::uint32_t>(std::stoul(argv[++i]));
        } else if (a == "--jsonl") {
            opt.jsonl = true;
        } else if (a == "--no-skip") {
            opt.skip_non_prime_p = false;
        } else {
            opt.exps_args.push_back(a);
        }
    }
    return opt;
}

}
