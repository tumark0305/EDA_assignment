#ifndef PIC_FILE_H
#define PIC_FILE_H
#include "global_variable.h"
#include "os_info.h"
#include <iostream>
#include <fstream>
#include <limits.h> 
#include <cstring> 
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cctype> 
#include <array>

using namespace std;
class PIC_file {
private:
	string current_path;
	string source_text;
	vector<string> split(string input_word); 
	void new_line(string new_line_data);
public:
	int grid[2];
	int propagation_loss;
	int crossing_loss;
	int bending_loss;
	int num_net;
	vector<net_info> data;
	PIC_file();
	bool read_fromfile(string input_file_name);
	void write_tofile(string output_file_name);
	bool read_fromdata(vector<net_info> input_data);
};

#endif
