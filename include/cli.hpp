#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace mersenne {

struct CliOptions {
    std::string input_file;             // --input path
    std::vector<std::string> exps_args;  // positional exponents
    std::uint32_t threads = 0;           // 0 => auto
    bool jsonl = false;                  // --jsonl
    bool skip_non_prime_p = true;        // --no-skip
};

CliOptions parse_cli(int argc, char** argv);
void print_help(const char* prog);

}
