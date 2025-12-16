FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
  g++ cmake make pkg-config \
  libgmp-dev libgmpxx4ldbl \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build -j

CMD ["./build/mersenne", "--help"]
