#ifndef GLOBAL_VARIABLE_H 
#define GLOBAL_VARIABLE_H 
#include <iostream>
#include <vector>
using namespace std;
struct header_info {
    string VERSION = "None";
    string DIVIDERCHAR = "None";
    string BUSBITCHARS = "None";
    string DESIGN = "None";
    unsigned int UNITS_DISTANCE_MICRONS = 0;
    string PROPERTYDEFINITIONS = "None";
    int DIEAREA[2][2] = { {0,0},{0,0} };
};
struct component_info {
    string inst_name = "None";
    string macro_name = "None";
    string place = "None";
    int coordinate[2] = { 0, 0 }; //x,y
    string orientation = "None";//N=left bottom
};
struct specialnet_info {
    string name = "None";
    string rout = "None";
    string layer = "None";
    int width = 0;
    int coordinate[2] = { 0, 0 }; //x,y
    int indirect_coordinate[2] = { 0,0 }; //x,y
};
struct command_info {
    string row_name = "None";
    string site_name = "None";
    int coordinate[2]  = { 0,0 };
    string orientation = "None";
    int num_size[2] = { 0,0 };
    int step[2] = { 0,0 };
};
struct data_info {
    header_info header;
    vector<component_info> component;
    vector< specialnet_info> specialnet;
};
#endif
