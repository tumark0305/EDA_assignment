#include "cuda_include.cuh"
#define array_length 10
//max blocks is 1104
__global__ void oddEvenSort(double* arr, int n) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) { // 案计近Ω矪瞶案计ま
            if (tid % 2 == 0 && tid + 1 < n) {
                if (arr[tid] > arr[tid + 1]) {
                    double temp = arr[tid];
                    arr[tid] = arr[tid + 1];
                    arr[tid + 1] = temp;
                }
            }
        }
        else { // 计近Ω矪瞶计ま
            if (tid % 2 == 1 && tid + 1 < n) {
                if (arr[tid] > arr[tid + 1]) {
                    double temp = arr[tid];
                    arr[tid] = arr[tid + 1];
                    arr[tid + 1] = temp;
                }
            }
        }
        __syncthreads(); // 玂靡–近ユ传常ЧΘ秈近
    }
}
double* cuda_main(double* mat_input) {
    GPU_info GPU;
    cout << "Run ON: " << GPU.data.name << endl;
    GPU.prt();
    double* d_arr, * matA;
    matA = (double*)malloc(array_length * sizeof(double));
    if (!matA) {
        std::cerr << "mamory malloc failed\n";
    }
    for (int i = 0; i < array_length; i++) {
        matA[i] = mat_input[i];
    }

    cudaMalloc(&d_arr, array_length * sizeof(double));
    cudaMemcpy(d_arr, matA, array_length * sizeof(double), cudaMemcpyHostToDevice);
    int num_blocks = (array_length + GPU.BLOCK_SIZE - 1) / GPU.BLOCK_SIZE;
    oddEvenSort << <num_blocks, GPU.BLOCK_SIZE >> > (d_arr, array_length);
    cudaMemcpy(matA, d_arr, array_length * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_arr);
    return matA;
}

