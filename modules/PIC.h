#ifndef PIC_H
#define PIC_H
#include "../cuda_include.cuh"

#include "global_variable.h"
#include "PIC_file.h"
#include <vector>
#include <array>
#include <set>
#include <cmath>
#include <algorithm> 
#include <string>
#include <limits>
#include <iostream>
#include <string>
#include <numeric>

using namespace std;
class PIC {
private:
	PIC_file header;
	bool use_cuda;
	int loss_cal_cross();
	int loss_cal_bend();
	int loss_cal_length();
	void convert_toline();
	vector<array<int, 2>> cross_at(const net_info& pina, const net_info& pinb);
	vector<vector<array<int, 2>>> direct_connection(const array<array<int, 2>, 2>& pin_location);
	vector<net_info> best_direct_connect_in_list(vector<net_info> pin_list);
	vector<net_info> from_smallest_direct_connect_in_list(vector<net_info> pin_list);
	vector<net_info> best_direct_connect_in_list_CUDA(vector<net_info> pin_list);
public:
	vector<net_info> data;
	PIC(PIC_file input , bool use_cuda_bool);
	double loss();
	void direct_connect();
};

#endif
