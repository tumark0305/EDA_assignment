#ifndef FINAL_FILE_H
#define FINAL_FILE_H
#include "os_info.h"
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
    int indirect_coordinate[2] = { 0,0 }; //x,y
};
struct data_info {
    header_info header;
    vector<component_info> component;
    vector< specialnet_info> specialnet;
};
class final_file {
private:
    unsigned int line_counter = 0;
    unsigned int components_len = 0;
    unsigned int specialnets_len = 0;
    string path;
    string line_tag = "OFF";
    string name;
    string get_bykeyword(string word, char key);
    string buffer = "Uncleaned buffer";
    bool PROPERTYDEFINITIONS = false;
    header_info header;
    vector<component_info> component;
    vector< specialnet_info> specialnet;
    void input_line(string new_line);
    void put_header(string new_line);
    void put_component(string new_line);
    void put_specialnet(string new_line);
public:
    void read_fromfile(string file_name);
    string head_text = "", components_text = "", specialnets_text = "";
    static data_info data_pack;
    final_file();
};
#endif