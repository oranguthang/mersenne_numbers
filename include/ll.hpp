#pragma once
#include <cstdint>

namespace mersenne {

// Basic primality check for exponent p (to skip LL when p is composite)
bool is_prime_u64(std::uint64_t n);

// Lucas-Lehmer test for M_p = 2^p - 1
// Returns true if M_p is prime (p must be prime; function will still work but is pointless otherwise)
bool lucas_lehmer(std::uint64_t p);

}
