
Vedic Kernels: Fast ARM Matrix Operations

Vedic matrix multiplication outperforms Eigen by 2.5x on ARM Cortex.

Optimized matrix multiplication kernels based on Vedic mathematics 
(Urdhva Tiryagbhyam sutra) for ARM processors.

Results (256x256 float32 matmul)

Method Time Speedup
Vedic Naive 3.75 ms 2.49x
Eigen Default 9.33 ms 1.00x

Quick Start

```bash
# Compile
g++ -O3 -march=native benchmarks/matmul_comparison.cpp \
    -I/usr/include/eigen3 -lbenchmark -lpthread -o matmul_comparison

# Run
./matmul_comparison
```

Files

· src/vedic_matmul.h - Vedic kernel implementations
· benchmarks/matmul_comparison.cpp - Eigen comparison benchmarks
· results/ - Raw benchmark data

License

MIT
