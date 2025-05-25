#include "cuda_include.cuh"
cudaError_t cudaStatus;
GPU_info::GPU_info() {
    cudaDeviceProp prop;
    int device;
    cudaGetDevice(&device);
    cudaGetDeviceProperties(&prop, device);
    data = prop;
    BLOCK_SIZE = prop.maxThreadsPerBlock;
}
void GPU_info::prt() {
    std::cout << "name" << data.name << std::endl;
    std::cout << "ability" << data.major << "." << data.minor << std::endl;
    std::cout << "maxThreadsPerBlock" << data.maxThreadsPerBlock << std::endl;
    std::cout << "maxGridSize"
        << data.maxGridSize[0] << " x "
        << data.maxGridSize[1] << " x "
        << data.maxGridSize[2] << std::endl;
    std::cout << "maxThreadsDim"
        << data.maxThreadsDim[0] << " x "
        << data.maxThreadsDim[1] << " x "
        << data.maxThreadsDim[2] << std::endl;
    std::cout << "multiProcessorCount" << data.multiProcessorCount << std::endl;
    std::cout << "maxBlocksPerMultiProcessor" << data.maxBlocksPerMultiProcessor << std::endl;
    std::cout << "maxThreadsPerMultiProcessor" << data.maxThreadsPerMultiProcessor << std::endl;
    std::cout << "sharedMemPerBlock" << data.sharedMemPerBlock / 1024.0 << " KB" << std::endl;
    std::cout << "totalGlobalMem" << data.totalGlobalMem / (1024.0 * 1024.0) << " MB" << std::endl;
}

