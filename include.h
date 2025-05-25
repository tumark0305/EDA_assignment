/*test area*/


#ifndef INCLUDE_H
#define INCLUDE_H
#include "modules/global_variable.h"
#include "modules/os_info.h"
#include "modules/def_file.h"
#include "modules/Legalization.h"
#include "modules/PIC.h"


#include <iostream>
#include <fstream>
#include <limits.h> 
#include <cstring> 
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <array>

using namespace std;
extern string input_file_name;
extern string output_file_name;
extern unsigned int cell_width;
extern float quality_alpha;
extern unsigned int cell_height;

bool get_parameters(int argc, char* argv[]);

class test {
private:
    unsigned int mat_length = 0;
public:
    void prt(double* mat);
    double* new_mat();
    double* matA = nullptr;
    double* matB = nullptr;
    double* matC = nullptr;
    test(unsigned int length);
};

class plot_info {
private:
    data_info data;
    string convert_plot();
    unsigned int component_size[2];
public:
    static string command;
    plot_info(data_info data_input, unsigned int size_input[2]);
    void prt();
    void write(string file_name);
};
#endif