#include <benchmark/benchmark.h>
#include <Eigen/Dense>
#include <vector>
#include <random>
#include <cstring>
#include "../src/vedic_matmul.h"

// Initialize matrices with random values
void init_matrices(float* A, float* B, int N) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    
    for (int i = 0; i < N * N; i++) {
        A[i] = dis(gen);
        B[i] = dis(gen);
    }
}

// Verify results match
bool verify_results(const float* C1, const float* C2, int N, float tolerance = 1e-3f) {
    for (int i = 0; i < N * N; i++) {
        if (std::abs(C1[i] - C2[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

// ============================================================
// BENCHMARKS
// ============================================================

// Vedic Urdhva - Naive
static void BM_Vedic_Naive(benchmark::State& state) {
    const int N = state.range(0);
    std::vector<float> A(N*N), B(N*N), C(N*N);
    init_matrices(A.data(), B.data(), N);
    
    for (auto _ : state) {
        vedic::VedicMatMul::urdhva_naive(A.data(), B.data(), C.data(), N);
        benchmark::DoNotOptimize(C);
    }
    state.SetComplexityN(N);
}
BENCHMARK(BM_Vedic_Naive)
    ->DenseRange(16, 256, 16)
    ->Complexity(benchmark::oNCubed);

// Vedic Urdhva - Optimized (blocked)
static void BM_Vedic_Optimized(benchmark::State& state) {
    const int N = state.range(0);
    std::vector<float> A(N*N), B(N*N), C(N*N);
    init_matrices(A.data(), B.data(), N);
    
    for (auto _ : state) {
        vedic::VedicMatMul::urdhva_optimized(A.data(), B.data(), C.data(), N);
        benchmark::DoNotOptimize(C);
    }
    state.SetComplexityN(N);
}
BENCHMARK(BM_Vedic_Optimized)
    ->DenseRange(16, 256, 16)
    ->Complexity(benchmark::oNCubed);

// Vedic Urdhva - SIMD
static void BM_Vedic_SIMD(benchmark::State& state) {
    const int N = state.range(0);
    std::vector<float> A(N*N), B(N*N), C(N*N);
    init_matrices(A.data(), B.data(), N);
    
    for (auto _ : state) {
        vedic::VedicMatMul::urdhva_simd(A.data(), B.data(), C.data(), N);
        benchmark::DoNotOptimize(C);
    }
    state.SetComplexityN(N);
}
BENCHMARK(BM_Vedic_SIMD)
    ->DenseRange(16, 256, 16)
    ->Complexity(benchmark::oNCubed);

// Eigen - Default
static void BM_Eigen_Default(benchmark::State& state) {
    const int N = state.range(0);
    Eigen::MatrixXf A(N, N), B(N, N), C(N, N);
    
    // Initialize
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    A = Eigen::MatrixXf::NullaryExpr(N, N, [&]() { return dis(gen); });
    B = Eigen::MatrixXf::NullaryExpr(N, N, [&]() { return dis(gen); });
    
    for (auto _ : state) {
        C.noalias() = A * B;
        benchmark::DoNotOptimize(C);
    }
    state.SetComplexityN(N);
}
BENCHMARK(BM_Eigen_Default)
    ->DenseRange(16, 256, 16)
    ->Complexity(benchmark::oNCubed);

// Eigen - With OpenMP
static void BM_Eigen_OpenMP(benchmark::State& state) {
    const int N = state.range(0);
    Eigen::MatrixXf A(N, N), B(N, N), C(N, N);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    A = Eigen::MatrixXf::NullaryExpr(N, N, [&]() { return dis(gen); });
    B = Eigen::MatrixXf::NullaryExpr(N, N, [&]() { return dis(gen); });
    
    for (auto _ : state) {
        C = A * B;  // Eigen uses OpenMP internally
        benchmark::DoNotOptimize(C);
    }
    state.SetComplexityN(N);
}
BENCHMARK(BM_Eigen_OpenMP)
    ->DenseRange(16, 256, 16)
    ->Complexity(benchmark::oNCubed);

BENCHMARK_MAIN();
