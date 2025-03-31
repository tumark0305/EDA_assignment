#ifndef INCLUDE_H
#define INCLUDE_H

#include <iostream>
#include <fstream>
#include <limits.h> 
#include <unistd.h>
#include <cstring> 
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <array>
#include <algorithm> 
using namespace std;
extern string input_file_name;
extern string output_file_name;
extern unsigned int component_size[2];
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

struct header_info {
    string VERSION = "None";
    string DIVIDERCHAR = "None";
    string BUSBITCHARS = "None";
    string DESIGN = "None";
    unsigned int UNITS_DISTANCE_MICRONS = 0;
    string PROPERTYDEFINITIONS = "None";
    int DIEAREA[2][2];
};
struct component_info {
    string name = "None";
    string tag = "None";
    string place = "None";
    int coordinate[2] = { 0, 0 }; //x,y
    string notation = "None";//N=left bottom
};
struct specialnet_info {
    string name = "None";
    string rout = "None";
    string layer = "None";
    int width = 0;
    int coordinate[2] = { 0, 0 }; //x,y
    int length[2] = { 0,0 }; //x,y
};
struct data_info {
    header_info header;
    vector<component_info> component;
    vector< specialnet_info> specialnet;
};

class os {
private:

public:
    string path;
    os();
};
extern os current;
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

class input_file {
private:
    unsigned int line_counter = 0;
    unsigned int components_len = 0;
    unsigned int specialnets_len = 0;
    string line_tag = "OFF";
    string name;
    string get_bykeyword(string word, char key);
    string buffer = "Dirty here--------";
    bool PROPERTYDEFINITIONS = false;
    header_info header;
    vector<component_info> component;
    vector< specialnet_info> specialnet;
    void input_line(string new_line);
    void put_header(string new_line);
    void put_component(string new_line);
    void put_specialnet(string new_line);

public:
    string head_text = "", components_text = "", specialnets_text = "";
    static data_info data_pack;
    input_file(string file_name);
};
#endif
