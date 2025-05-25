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
vector<array<int, 2>> spring_cuda(vector< BlockInfo>& block_list_input);
bool no_overlap_cuda(vector<BlockInfo>& block_list_input);
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