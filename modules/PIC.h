#ifndef PIC_H
#define PIC_H
#include "global_variable.h"
#include "PIC_file.h"
#include <vector>
#include <array>
#include <set>
#include <cmath>
#include <algorithm> 

using namespace std;
class PIC {
private:
	PIC_file header;
	vector<net_info> data; 
	int loss_cal_cross();
	int loss_cal_bend();
	int loss_cal_length();
	void convert_toline();
public:
	PIC(PIC_file input);
	vector<array<int, 2>> cross_at(const net_info& pina, const net_info& pinb);
	vector<vector<array<int, 2>>> direct_connection(const array<array<int, 2>, 2>& pin_location); 
	double loss();
};

#endif
