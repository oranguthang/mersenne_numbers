#include "ll.hpp"
#include <gmpxx.h>

namespace mersenne {

bool is_prime_u64(std::uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (std::uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

bool lucas_lehmer(std::uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;

    mpz_class M = (mpz_class(1) << p) - 1; // 2^p - 1
    mpz_class s = 4;

    for (std::uint64_t i = 0; i < p - 2; ++i) {
        s = (s * s - 2) % M;
    }
    return s == 0;
}

}
