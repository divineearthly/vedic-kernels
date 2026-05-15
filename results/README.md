# Vedic vs Eigen - ARM Cortex Benchmark Results

## 256x256 Matrix Multiplication (float32)

| Implementation | Time (ns) | Speedup vs Eigen |
|---------------|-----------|------------------|
| **Vedic Naive (Urdhva)** | 3,749,405 | **2.49x** |
| Vedic SIMD | 3,985,823 | 2.34x |
| Vedic Optimized (blocked) | 4,842,808 | 1.93x |
| Eigen with OpenMP | 4,445,918 | 2.10x |
| Eigen Default | 9,332,803 | 1.00x |

## Test Environment
- CPU: 8-core ARM Cortex @ 1958 MHz
- OS: Ubuntu (Termux/Proot)
- Compiler: GCC 15.2.0 (-O3 -march=native -flto)
- Eigen version: 3.4.0
- Benchmark framework: Google Benchmark v1.9.5

## Key Finding
Vedic Urdhva Tiryagbhyam matrix multiplication outperforms 
Eigen's default implementation by 2.5x on ARM Cortex for 
256x256 float32 matrices.

## Reproduction
```bash
g++ -O3 -march=native benchmarks/matmul_comparison.cpp \
    -I/usr/include/eigen3 -lbenchmark -lpthread \
    -o matmul_comparison
./matmul_comparison
```

