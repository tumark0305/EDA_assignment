#ifndef INCLUDE_H
#define INCLUDE_H

#include <iostream>
#include <fstream>
#include <limits.h> 
//#include <unistd.h>
#include <cstring> 
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <array>

#ifdef _WIN32
#include <windows.h>
#define usleep(x) Sleep((x) / 1000)
#else
#include <unistd.h>
#endif
using namespace std; 

class os {
private:

public:
    string path;
    os();
};

class input_file {
private:
    unsigned int line_counter = 0;
    unsigned int components_len = 0;
    unsigned int specialnets_len = 0;
    string line_tag = "OFF";
    string name;
    string get_bykeyword(string word , char key);
    string buffer = "Dirty here--------";
    bool PROPERTYDEFINITIONS = false;
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
        int coordinate[2] = {0, 0}; //x,y
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

    void input_line(string new_line);
    void put_header(string new_line);
    void put_component(string new_line);
    void put_specialnet(string new_line);

public:
    string head_text="", components_text = "", specialnets_text = "";
    header_info header;
    vector<component_info> component;
    vector< specialnet_info> specialnet;
    
    input_file(string file_name);
};

#endif