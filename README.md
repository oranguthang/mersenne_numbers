# mersenne (C++ / GMP)

Lucas-Lehmer primality test for Mersenne numbers: M_p = 2^p - 1. CLI supports file/args input, multithreading, table or JSONL output. C++20 + GMP.

## Features
- Lucas-Lehmer on `mpz_class` (GMP)
- Multi-threaded work queue (`std::thread`, atomic index)
- Inputs: positional exponents or `--input` file (supports commas and `#` comments)
- Outputs: pretty table (default) or JSON Lines (`--jsonl`)
- Skip composite exponents by default (`--no-skip` to force LL)

## Dependencies
- C++20 compiler (g++, clang, MSVC)
- CMake >= 3.20
- GMP + gmpxx

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y g++ cmake make libgmp-dev libgmpxx4ldbl
```

### macOS (Homebrew)
```bash
brew install gmp
```

### Windows (vcpkg manifest mode)
```pwsh
# One-time: clone + bootstrap vcpkg
# git clone https://github.com/microsoft/vcpkg.git
# .\vcpkg\bootstrap-vcpkg.bat

# Configure + build with toolchain
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```
A `vcpkg.json` is included; CMake + toolchain will install `gmp` automatically.

## Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

## Usage
From CLI args:
```bash
./build/mersenne 31 61 89
./build/mersenne 127,521,607 --jsonl
./build/mersenne 31 61 --threads 4
```
From file:
```bash
./build/mersenne --input exponents.txt --threads 8
```
Help:
```bash
./build/mersenne --help
```

## Input file format
- Integers separated by space/tab/comma
- `#` comments allowed (full line or inline)

Example `exponents.txt`:
```
# classic small ones
2 3 5 7 13
17 19 31
# bigger
61
89
127
```

## Output
Table (default):
```
mersenne 0.1.0 | threads=4 | items=6 | run_id=1702755842000

p         p prime?    M_p prime?        time(s)     note
------------------------------------------------------------
2         yes         PRIME             0.0000
3         yes         PRIME             0.0000
5         yes         PRIME             0.0000
61        yes         composite         0.0001
89        yes         composite         0.0001
127       yes         PRIME             0.0001
```

JSON Lines (`--jsonl`):
```json
{"p":2,"p_is_prime":true,"mersenne_is_prime":true,"seconds":0.000023,"note":""}
{"p":89,"p_is_prime":true,"mersenne_is_prime":false,"seconds":0.123456,"note":""}
```

## Docker
```bash
docker build -t mersenne .
docker run --rm mersenne
```

## Windows quick-start (PowerShell 7)
```pwsh
# install/update PowerShell 7 if needed: winget install Microsoft.PowerShell

# clone + bootstrap vcpkg
# git clone https://github.com/microsoft/vcpkg.git
# cd vcpkg; .\bootstrap-vcpkg.bat

# back to project
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## License
MIT (see LICENSE)
