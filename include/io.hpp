#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace mersenne {

std::vector<std::uint64_t> parse_exponents_from_args(const std::vector<std::string>& args);
std::vector<std::uint64_t> read_exponents_file(const std::string& path);

}
