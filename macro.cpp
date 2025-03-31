#include "include.h"

string getcurrentpath() { 
    char path[1024];

#ifdef _WIN32
    GetCurrentDirectoryA(sizeof(path), path);
#else
    getcwd(path, sizeof(path));
#endif
    string output = path;
    if (output.find("Debug") != string::npos) {
        size_t space_pos0 = output.rfind("\\");
        output = output.substr(0 , space_pos0);
    }
    
    return output;
}

int cstoi(string x) {
    try {
        int y = stoi(x); 
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Number:###" << x <<"###"<< std::endl;
        cerr << "Invalid argument: " << e.what() << endl;
    }
    int y = 10;
    return y;
}

os::os() {
    path = getcurrentpath();
}

input_file::input_file(string file_name) : name(file_name) {
    os current;
    string input_path = current.path + "/" + file_name;
    string line;
    ifstream file(input_path);
    if (!file) {
        cerr << "cannot open file!" << endl;
    }

    while (getline(file, line)) {
        input_line(line);
    }
    file.close();
    cout << "nicec" << endl;
}
void input_file::input_line(string new_line){
    if (line_tag == "OFF" && new_line.find("VERSION") != string::npos) {
        line_tag = "head";
        head_text += "\n" + new_line;
    }
    else if (line_tag == "head") {
        if (new_line.find("COMPONENTS") != string::npos) {
            line_tag = "components";
            components_text += "\n" + new_line;
            components_len = cstoi(get_bykeyword(new_line, ' '));
        }
        else {
            head_text += "\n" + new_line;
            put_header(new_line);
        }
    }
    else if (line_tag == "components") {
        components_text += "\n" + new_line;
        if (new_line.find(";") != string::npos) {
            put_component(buffer + new_line);
        }
        else {
            buffer = new_line;
        }
        if (new_line == "END COMPONENTS") {
            line_tag = "OFF";
        }
    }
    else if (line_tag == "OFF" && new_line.find("SPECIALNETS") != string::npos) {
        line_tag = "specialnets";
        specialnets_text += "\n" + new_line;
        specialnets_len = cstoi(get_bykeyword(new_line, ' '));
    }
    else if (line_tag == "specialnets") {
        specialnets_text += "\n" + new_line;
        if (new_line.find(";") != string::npos) {
            put_specialnet(buffer + new_line); 
        }
        else {
            buffer = new_line;
        }
        if (new_line == "END SPECIALNETS") {
            line_tag = "OFF";
        }
    }
}
void input_file::put_header(string new_line) {
    
    /*
    output.VERSION = "Y";
    output.DIVIDERCHAR = "Y";
    output.BUSBITCHARS = "Y";
    output.DESIGN = "Y";
    output.UNITS_DISTANCE_MICRONS = "Y";
    output.PROPERTYDEFINITIONS = "Y";
    output.DIEAREA = "Y";*/
    if (new_line.find("VERSION") != string::npos) {
        header.VERSION = get_bykeyword(new_line, ' ');
    }
    else if (new_line.find("DIVIDERCHAR") != string::npos) {
        header.DIVIDERCHAR = get_bykeyword(new_line, '"');
    }
    else if (new_line.find("BUSBITCHARS") != string::npos) {
        header.BUSBITCHARS = get_bykeyword(new_line, '"');
    }
    else if (new_line.find("DESIGN") != string::npos) {
        header.DESIGN = get_bykeyword(new_line, ' ');
    }
    else if (new_line.find("UNITS DISTANCE MICRONS") != string::npos) {
        header.UNITS_DISTANCE_MICRONS = cstoi(get_bykeyword(new_line, ' '));
    }
    else if (new_line == "PROPERTYDEFINITIONS") {
        PROPERTYDEFINITIONS = true;
    }
    else if (new_line == "END PROPERTYDEFINITIONS") {
        PROPERTYDEFINITIONS = false;
    }
    else if (PROPERTYDEFINITIONS) {
        size_t space_pos1 = new_line.rfind(" ");
        size_t space_pos0 = new_line.find(" ");
        if (space_pos0 > new_line.length() || space_pos1 > new_line.length()) {
            ostringstream error_message;
            error_message << "space_pos0 = " << space_pos0 << "\nspace_pos1 = " << space_pos1;
            throw std::runtime_error(error_message.str());
        }
        header.PROPERTYDEFINITIONS = new_line.substr(space_pos0 +2, space_pos1 - space_pos0 - 2);
    }
    else if (new_line.find("DIEAREA") != string::npos) {
        size_t space_pos1 = new_line.rfind(" ");
        size_t space_pos0 = new_line.find(" ");
        if (space_pos0 > new_line.length() || space_pos1 > new_line.length()) {
            ostringstream error_message;
            error_message << "space_pos0 = " << space_pos0 << "\nspace_pos1 = " << space_pos1;
            throw std::runtime_error(error_message.str());
        }
        string str_cord = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
        space_pos1 = 0;
        for (unsigned int x = 0; x < 2; x++) {
            for (unsigned int y = 0; y < 2; y++) {
                space_pos0 = str_cord.find(" ", space_pos1);
                space_pos1 = str_cord.find(" ", space_pos0 + 1);
                header.DIEAREA[x][y] = cstoi(str_cord.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
            }
            space_pos1 += 3;
        }
    }
    //return header;;
}
void input_file::put_component(string new_line) {
    component_info _info; 
    string  _coordinate_text;
    size_t space_pos1 = 0;
    size_t space_pos0 = 0;
    space_pos0 = new_line.find(" ", space_pos1);
    space_pos1 = new_line.find(" ", space_pos0 + 1);
    _info.name = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    space_pos0 = new_line.find(" ", space_pos1); 
    space_pos1 = new_line.find(" ", space_pos0 + 1); 
    _info.tag = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    space_pos0 = new_line.find(" ", space_pos1+2);
    space_pos1 = new_line.find(" ", space_pos0 + 1);
    _info.place = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    space_pos0 = new_line.find("(", space_pos1);
    space_pos1 = new_line.find(")", space_pos0 + 1);
    _coordinate_text = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    space_pos0 = new_line.find(" ", space_pos1);
    space_pos1 = new_line.find(" ", space_pos0 + 1);
    _info.notation = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    space_pos1 = 0;
    space_pos0 = _coordinate_text.find(" ", space_pos1);
    space_pos1 = _coordinate_text.find(" ", space_pos0 + 1);
    _info.coordinate[0] = cstoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
    space_pos0 = _coordinate_text.find(" ", space_pos1);
    space_pos1 = _coordinate_text.find(" ", space_pos0 + 1);
    _info.coordinate[1] = cstoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
    component.push_back(_info); 
}
void input_file::put_specialnet(string new_line) {
    specialnet_info _info; 
    string  _coordinate_text,_length_text;
    size_t space_pos1 = 0;
    size_t space_pos0 = 0;
    space_pos0 = new_line.find(" ", space_pos1);
    space_pos1 = new_line.find(" ", space_pos0 + 1);
    _info.name = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    
    space_pos0 = new_line.find(" ", space_pos1 + 2);
    space_pos1 = new_line.find(" ", space_pos0 + 1);
    _info.rout = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    
    space_pos0 = new_line.find(" ", space_pos1);
    space_pos1 = new_line.find(" ", space_pos0 + 1);
    _info.layer = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    
    space_pos0 = new_line.find(" ", space_pos1);
    space_pos1 = new_line.find(" ", space_pos0 + 1);
    _info.width = cstoi(new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1)); 
    
    space_pos0 = new_line.find("(", space_pos1);
    space_pos1 = new_line.find(")", space_pos0 + 1);
    _coordinate_text = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    
    space_pos0 = new_line.find("(", space_pos1);
    space_pos1 = new_line.find(")", space_pos0 + 1);
    _length_text = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    space_pos1 = 0;
    for (int x0 = 0; x0 < 2; x0++) {
        space_pos0 = _coordinate_text.find(" ", space_pos1);
        space_pos1 = _coordinate_text.find(" ", space_pos0 + 1);
        _info.coordinate[x0] = cstoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
    }
    space_pos1 = 0;
    for (int x1 = 0; x1 < 2; x1++) {
        space_pos0 = _length_text.find(" ", space_pos1);
        space_pos1 = _length_text.find(" ", space_pos0 + 1);
        string _length = _length_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1); 
        if (_length == "*") {
            _info.length[x1] = 0;
        }
        else {
            _info.length[x1] = cstoi(_length);
        }
    }
    specialnet.push_back(_info); 
}
string input_file::get_bykeyword(string word, char key) {
    string output = "not find";
    size_t space_pos1 = word.rfind(key);
    size_t space_pos0 = word.rfind(key, space_pos1 - 1);
    if (space_pos0 > word.length() || space_pos1 > word.length()) {
        ostringstream error_message;
        error_message << "space_pos0 = " << space_pos0 << "\nspace_pos1 = " << space_pos1;
        throw std::runtime_error(error_message.str());
    }
    output = word.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    //cout << output << endl;
    return output;
}