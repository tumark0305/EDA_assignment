// CUDA kernel and function for evaluating best selector (spring-style architecture)
#include "cuda_include.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <array>
#include <vector>
#include <climits>

using namespace std;

__device__ int count_crosses(
    const int* path_a, int len_a,
    const int* path_b, int len_b
) {
    int cross = 0;
    for (int i = 0; i < len_a / 4; ++i) {
        for (int j = 0; j < len_b / 4; ++j) {
            bool match_start = path_a[i * 4 + 0] == path_b[j * 4 + 0] && path_a[i * 4 + 1] == path_b[j * 4 + 1];
            bool match_end = path_a[i * 4 + 2] == path_b[j * 4 + 2] && path_a[i * 4 + 3] == path_b[j * 4 + 3];
            if (match_end) ++cross;
        }
    }
    return cross;
}

__global__ void evaluate_selectors_kernel(
    int N,
    const int* option_flat,
    const int* option_lengths,
    const int* option_offsets,
    int max_path_len,
    int* cross_counts
) {
    int selector = blockIdx.x * blockDim.x + threadIdx.x;
    if (selector >= (1 << N)) return;

    __shared__ int local_paths[64 * 128]; // 64 nets x 128 max length

    int* thread_paths = &local_paths[threadIdx.x * N * max_path_len];

    for (int i = 0; i < N; ++i) {
        int option_id = ((selector >> i) & 1);
        int index = i * 2 + option_id;
        int offset = option_offsets[index];
        int len = option_lengths[index];
        for (int j = 0; j < len; ++j) {
            thread_paths[i * max_path_len + j] = option_flat[offset + j];
        }
    }

    int total_cross = 0;
    for (int i = 0; i < N; ++i) {
        int len_a = option_lengths[i * 2 + ((selector >> i) & 1)];
        int* path_a = &thread_paths[i * max_path_len];
        for (int j = i + 1; j < N; ++j) {
            int len_b = option_lengths[j * 2 + ((selector >> j) & 1)];
            int* path_b = &thread_paths[j * max_path_len];
            total_cross += count_crosses(path_a, len_a, path_b, len_b);
        }
    }

    cross_counts[selector] = total_cross;
}

std::vector<int> evaluate_crossing_cuda(
    std::vector<int>& option_flat,
    std::vector<int>& option_lengths,
    std::vector<int>& option_offsets,
    int N,
    int max_path_len
) {
    int selector_count = 1 << N;
    std::vector<int> cross_counts(selector_count);

    int* d_option_flat;
    int* d_option_lengths;
    int* d_option_offsets;
    int* d_cross_counts;

    cudaMalloc(&d_option_flat, option_flat.size() * sizeof(int));
    cudaMalloc(&d_option_lengths, option_lengths.size() * sizeof(int));
    cudaMalloc(&d_option_offsets, option_offsets.size() * sizeof(int));
    cudaMalloc(&d_cross_counts, selector_count * sizeof(int));

    cudaMemcpy(d_option_flat, option_flat.data(), option_flat.size() * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_option_lengths, option_lengths.data(), option_lengths.size() * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_option_offsets, option_offsets.data(), option_offsets.size() * sizeof(int), cudaMemcpyHostToDevice);

    int threads = 128;
    int blocks = (selector_count + threads - 1) / threads;

    evaluate_selectors_kernel << <blocks, threads >> > (
        N, d_option_flat, d_option_lengths, d_option_offsets, max_path_len, d_cross_counts);

    cudaMemcpy(cross_counts.data(), d_cross_counts, selector_count * sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(d_option_flat);
    cudaFree(d_option_lengths);
    cudaFree(d_option_offsets);
    cudaFree(d_cross_counts);

    return cross_counts;
}