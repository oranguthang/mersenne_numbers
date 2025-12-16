#include "io.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace mersenne {

static bool is_comment_or_empty(const std::string& line) {
    for (char c : line) {
        if (c == '#') return true;
        if (!std::isspace(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

std::vector<std::uint64_t> parse_exponents_from_args(const std::vector<std::string>& args) {
    // Accept: "31 61 89" or "31,61,89" (mixed)
    std::vector<std::uint64_t> out;
    for (const auto& token : args) {
        std::string s = token;
        for (char& c : s) if (c == ',') c = ' ';
        std::istringstream iss(s);
        std::uint64_t p;
        while (iss >> p) out.push_back(p);
    }
    return out;
}

std::vector<std::uint64_t> read_exponents_file(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Failed to open input file: " + path);

    std::vector<std::uint64_t> out;
    std::string line;
    while (std::getline(in, line)) {
        if (is_comment_or_empty(line)) continue;

        // Allow inline comments after numbers
        auto pos = line.find('#');
        if (pos != std::string::npos) line = line.substr(0, pos);

        std::istringstream iss(line);
        std::uint64_t p;
        while (iss >> p) out.push_back(p);
    }
    return out;
}

}
