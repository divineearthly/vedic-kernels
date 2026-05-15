#ifndef VEDIC_MATMUL_H
#define VEDIC_MATMUL_H

#include <vector>
#include <cstring>
#include <algorithm>

namespace vedic {

class VedicMatMul {
public:
    // Standard Urdhva Tiryagbhyam - Vertically and Crosswise
    static void urdhva_naive(const float* A, const float* B, float* C, int N) {
        std::memset(C, 0, N * N * sizeof(float));
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                float aik = A[i * N + k];
                for (int j = 0; j < N; j++) {
                    C[i * N + j] += aik * B[k * N + j];
                }
            }
        }
    }
    
    // Cache-optimized with loop reordering
    static void urdhva_optimized(const float* A, const float* B, float* C, int N) {
        std::memset(C, 0, N * N * sizeof(float));
        constexpr int BLOCK = 32;
        
        for (int i0 = 0; i0 < N; i0 += BLOCK) {
            for (int k0 = 0; k0 < N; k0 += BLOCK) {
                for (int j0 = 0; j0 < N; j0 += BLOCK) {
                    for (int i = i0; i < std::min(i0 + BLOCK, N); i++) {
                        for (int k = k0; k < std::min(k0 + BLOCK, N); k++) {
                            float aik = A[i * N + k];
                            for (int j = j0; j < std::min(j0 + BLOCK, N); j++) {
                                C[i * N + j] += aik * B[k * N + j];
                            }
                        }
                    }
                }
            }
        }
    }
    
    // SIMD-friendly with aligned access pattern
    static void urdhva_simd(const float* A, const float* B, float* C, int N) {
        std::memset(C, 0, N * N * sizeof(float));
        
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                float aik = A[i * N + k];
                #pragma GCC ivdep
                for (int j = 0; j < N; j++) {
                    C[i * N + j] += aik * B[k * N + j];
                }
            }
        }
    }
};

} // namespace vedic
#endif
