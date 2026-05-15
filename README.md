# vedic-kernels

**High-performance C++ matrix operations based on Vedic Mathematics (Urdhva Tiryagbhyam)**  
Benchmarked on ARM64 mobile hardware. Competitive with Eigen on constrained devices.

---

## What This Is

Ancient Indian mathematics contains a matrix multiplication algorithm called **Urdhva Tiryagbhyam** ("vertically and crosswise") from the Vedic sutras. This repository implements that algorithm in modern C++ and benchmarks it against the industry-standard Eigen library — on ARM64 mobile hardware (the kind that runs India's rural infrastructure).

The result: **Vedic Naive outperforms Eigen Default by ~2.5× on 256×256 matrices on ARM64**, with lower variance.

This is not a metaphysical claim. It is a benchmark.

---

## Benchmark Results (256×256 Matrix Multiply, ARM64)

| Implementation | CPU Time (mean) | Std Dev | Notes |
|---|---|---|---|
| **Vedic Naive** | **3.75 ms** | 207 µs | Urdhva Tiryagbhyam |
| Vedic SIMD | 3.99 ms | 130 µs | NEON-assisted |
| Eigen Default | 9.33 ms | 2826 µs | High variance on ARM64 |
| Eigen OpenMP | 4.45 ms | 223 µs | Multi-threaded |

**Environment:**
- CPU: 8 cores @ 1958.4 MHz (ARM64)
- Compiler: GCC 15.2.0 with `-O3 -march=native -flto`
- Framework: Google Benchmark v1.9.5
- Repetitions: 10 per test

**Key observation:** Eigen showed 86% RMS error on its BigO fit vs. 22% for Vedic Naive, indicating Vedic Naive has more predictable, consistent scaling on this architecture.

Full scaling data: [results/vedic_vs_eigen.json](results/vedic_vs_eigen.json)

---

## Scaling Profile

| Size | Vedic Naive | Vedic SIMD | Eigen Default |
|---|---|---|---|
| 16×16 | 878 ns | 965 ns | ~750 ns |
| 64×64 | 38.8 µs | 47.5 µs | — |
| 128×128 | 421 µs | — | — |
| 256×256 | 3.75 ms | 3.99 ms | 9.33 ms |

All three implementations confirm **O(n³)** complexity as expected for general matrix multiply.

---

## Repository Structure

```
vedic-kernels/
├── src/
│   └── vedic_matmul.h          # Urdhva Tiryagbhyam implementation
├── benchmarks/
│   └── matmul_comparison.cpp   # Google Benchmark suite
├── results/
│   └── vedic_vs_eigen.json     # Raw benchmark data
└── docs/
```

---

## Building

```bash
# Dependencies
sudo apt install libbenchmark-dev libeigen3-dev

# Compile
g++ -O3 -march=native -flto benchmarks/matmul_comparison.cpp \
    -lbenchmark -lpthread -o matmul_comparison

# Run
./matmul_comparison
```

**ARM64/Termux (mobile):**
```bash
pkg install eigen benchmark
g++ -O3 -march=native benchmarks/matmul_comparison.cpp \
    -lbenchmark -lpthread -o matmul_comparison
./matmul_comparison
```

---

## The Algorithm: Urdhva Tiryagbhyam

The Vedic sutra *Urdhva Tiryagbhyam* ("By Rows and Columns") describes a multiplication approach where partial products are computed simultaneously and summed diagonally. In matrix form, this maps naturally to cache-friendly inner-product computation.

The algorithm's strength on ARM64 comes from predictable memory access patterns — sequential inner loops that align well with the cache prefetcher on mobile SoCs, unlike Eigen's default codepath which is optimized for x86 LAPACK-style blocked operations.

---

## Part of the Divine Earthly Project

This repository is the computation kernel for the [Divine-Earthly-ASI](https://github.com/divineearthly/Divine-Earthly-ASI) system — an offline-first agricultural AI designed for rural Indian farmers, built entirely on mobile hardware.

**Related repositories:**
- [Divine-Earthly-ASI](https://github.com/divineearthly/Divine-Earthly-ASI) — Main AI system with farmer query engines
- [Krishi-Veda-Module](https://github.com/divineearthly/Krishi-Veda-Module) — Agricultural inference module

---

## Honest Limitations

- Eigen's high variance in these results may reflect suboptimal configuration for this specific environment; Eigen is generally competitive or faster on x86
- SIMD implementation uses NEON intrinsics; further optimization possible
- Large matrix performance (>512×512) not yet fully characterized

---

## Author

**Joydeep Das** — Independent AI researcher, Silchar, Assam, India  
GitHub: [@divineearthly](https://github.com/divineearthly)  
Built entirely on Android/Termux (ARM64), no desktop hardware used.

---

## License

MIT License — use freely, cite if you publish.

---

*"Urdhva Tiryagbhyam — By Rows and Columns. The ancient sutra, the modern benchmark."*
