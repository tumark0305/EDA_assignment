#ifndef CUDA_INCLUDE_H
#define CUDA_INCLUDE_H
#include <iostream>
#include <stdio.h>
#include <sstream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <vector>
#include <array>
#include <cmath>
#include <cstdlib>
#include "modules/global_variable.h"
#include "modules/Legalization.h"
#include <curand_kernel.h>

using namespace std;
std::vector<int> evaluate_crossing_cuda(
	std::vector<int>& option_flat,
	std::vector<int>& option_lengths,
	std::vector<int>& option_offsets,
	int N,
	int max_path_len
);
extern cudaError_t cudaStatus;
class GPU_info {
private:
public:
	cudaDeviceProp data;
	GPU_info();
	void prt();
	int BLOCK_SIZE;
};

struct simple_BlockInfo {
	int2 coordinate;  // coordinate.x = x, coordinate.y = y
	int2 size;        // size.x = width, size.y = height
};

#endif