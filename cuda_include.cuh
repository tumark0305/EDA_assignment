#ifndef INCLUDE_H
#define INCLUDE_H
#include <iostream>
#include <stdio.h>
#include <sstream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

using namespace std;
double* cuda_main(double* mat_input);
extern cudaError_t cudaStatus;
class GPU_info {
private:
public:
	cudaDeviceProp data;
	GPU_info();
	void prt();
	int BLOCK_SIZE;
};
#endif