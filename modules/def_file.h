#ifndef DEF_FILE_H
#define DEF_FILE_H
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
#include <array>
using namespace std;
class def_file {
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
    vector< command_info> command;
    vector<component_info> component;
    vector< specialnet_info> specialnet;
    void input_line(string new_line);
    void put_header(string new_line);
    void put_command(string new_line);
    void put_component(string new_line);
    void put_specialnet(string new_line);
public:
    void read_fromfile(string file_name);
    string head_text = "", components_text = "", specialnets_text = "";
    static data_info data_pack;
    def_file();
};
#endif