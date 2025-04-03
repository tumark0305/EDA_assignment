#include "include.h"

os_info::os_info() {
    char buffer[1024];
#ifdef _WIN32
    DWORD length = GetCurrentDirectoryA(sizeof(buffer), buffer);
    if (length == 0) {
        std::cerr << "GetCurrentDirectory() failed with error: " << GetLastError() << std::endl;
    }
#else
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        perror("getcwd() error");
    }
#endif
    path = buffer;
}

bool get_parameters(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <MSSC width> <MSSC height> <input file> <output file>" << endl;
        return true;
    }
    else {
        component_size[0] = stoi(argv[1]);
        component_size[1] = stoi(argv[2]);
        input_file_name = argv[3];
        output_file_name = argv[4];
        return false;
    }
}
data_info input_file_final::data_pack; //all acessable
input_file_final::input_file_final(string file_name) : name(file_name) {
    os_info current;
    string input_path = current.path + "/" + file_name;
    string line;
    ifstream file(input_path);
    if (!file) {
        cerr << "cannot open file!" << input_path  << endl;
    }

    while (getline(file, line)) {
        for (auto it = line.begin(); it != line.end(); ) {
            if (*it == '\r') {
                it = line.erase(it);
            }
            else {
                ++it;
            }
        }
        input_line(line);
    }
    file.close();
    data_pack.header = header;
    data_pack.component = component;
    data_pack.specialnet = specialnet;
}
void input_file_final::input_line(string new_line) {
    if (line_tag == "OFF" && new_line.find("VERSION") != string::npos) {
        line_tag = "head";
        head_text += "\n" + new_line;
        put_header(new_line);
    }
    else if (line_tag == "head") {
        if (new_line.find("COMPONENTS") != string::npos) {
            line_tag = "components";
            components_text += "\n" + new_line;
            components_len = stoi(get_bykeyword(new_line, ' '));
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
        specialnets_len = stoi(get_bykeyword(new_line, ' '));
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
void input_file_final::put_header(string new_line) {
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
        header.UNITS_DISTANCE_MICRONS = stoi(get_bykeyword(new_line, ' '));
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
        header.PROPERTYDEFINITIONS = new_line.substr(space_pos0 + 2, space_pos1 - space_pos0 - 2);
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
                header.DIEAREA[x][y] = stoi(str_cord.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
            }
            space_pos1 += 3;
        }
    }
    //return header;;
}
void input_file_final::put_component(string new_line) {

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
    space_pos0 = new_line.find(" ", space_pos1 + 2);
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

    _info.coordinate[0] = stoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
    space_pos0 = _coordinate_text.find(" ", space_pos1);
    space_pos1 = _coordinate_text.find(" ", space_pos0 + 1);

    _info.coordinate[1] = stoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
    component.push_back(_info);
}
void input_file_final::put_specialnet(string new_line) {
    specialnet_info _info;
    string  _coordinate_text, _indirect_coordinate_text;
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
    _info.width = stoi(new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));

    space_pos0 = new_line.find("(", space_pos1);
    space_pos1 = new_line.find(")", space_pos0 + 1);
    _coordinate_text = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

    space_pos0 = new_line.find("(", space_pos1);
    space_pos1 = new_line.find(")", space_pos0 + 1);
    _indirect_coordinate_text = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    space_pos1 = 0;
    for (int x0 = 0; x0 < 2; x0++) {
        space_pos0 = _coordinate_text.find(" ", space_pos1);
        space_pos1 = _coordinate_text.find(" ", space_pos0 + 1);
        _info.coordinate[x0] = stoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
    }
    space_pos1 = 0;
    for (int x1 = 0; x1 < 2; x1++) {
        space_pos0 = _indirect_coordinate_text.find(" ", space_pos1);
        space_pos1 = _indirect_coordinate_text.find(" ", space_pos0 + 1);
        string _indirect_coordinate = _indirect_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
        if (_indirect_coordinate == "*") {
            _info.indirect_coordinate[x1] = _info.coordinate[x1];
        }
        else {
            _info.indirect_coordinate[x1] = stoi(_indirect_coordinate);
        }
    }
    specialnet.push_back(_info);
}
string input_file_final::get_bykeyword(string word, char key) {
    string output = "not find";
    size_t space_pos1 = word.rfind(key);
    size_t space_pos0 = word.rfind(key, space_pos1 - 1);
    if (space_pos0 > word.length() || space_pos1 > word.length()) {
        ostringstream error_message;
        error_message << "space_pos0 = " << space_pos0 << "\nspace_pos1 = " << space_pos1;
        throw std::runtime_error(error_message.str());
    }
    output = word.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
    //cout << space_pos0 << endl;
    //cout << space_pos1 << endl;
    //cout << output << endl;
    return output;
}
string plot_info::command = "";
plot_info::plot_info(data_info data_input, unsigned int size_input[2]) {
    data = data_input;
    component_size[0] = size_input[0];
    component_size[1] = size_input[1];
    command = "";
    command += "reset\nset title \"result\"\nset xlabel \"X\"\nset ylabel \"Y\"\n";
    command += convert_plot();
    command += "set xtics 10000\nset ytics 10000\nset size square\nset grid\nplot ";
    command += "[" + to_string(data.header.DIEAREA[0][0]) + ":" + to_string(data.header.DIEAREA[1][0]) + "][" + to_string(data.header.DIEAREA[0][1]) + ":" + to_string(data.header.DIEAREA[1][1]) + "]";
    command += "0\nset terminal png size 3840,2160\nset output \"output.png\"\nreplot\nreplot";

}
string plot_info::convert_plot() {
    unsigned int name_offset[2] = { component_size[0] / 2 , component_size[1] / 2 };
    string output = "";
    unsigned int tag_counter = 1;
    for (int component_index = 0; component_index < data.component.size(); component_index++) {
        output += "set object " + to_string(tag_counter) + " rect from ";
        output += to_string(data.component[component_index].coordinate[0]) + "," + to_string(data.component[component_index].coordinate[1]) + " to ";
        output += to_string(data.component[component_index].coordinate[0] + component_size[0]) + "," + to_string(data.component[component_index].coordinate[1] + component_size[1]) + " lw 1 fs solid fc rgb \"#c080ff\"\n";
        output += "set label \"" + data.component[component_index].tag + "\" at ";
        output += to_string(data.component[component_index].coordinate[0] + name_offset[0]) + "," + to_string(data.component[component_index].coordinate[1] + name_offset[1]) + " center\n";
        tag_counter++;
    }
    for (int specialnet_index = 0; specialnet_index < data.specialnet.size(); specialnet_index++) {
        unsigned int half_width = data.specialnet[specialnet_index].width / 2;
        unsigned int LB[2] = { 0,0 }, RT[2] = { 0,0 }, TEXT[2] = { 0,0 };
        string color = "None";
        if (data.specialnet[specialnet_index].layer == "ME4") {
            color = "ff8000";
        }
        else {
            color = "00ffff";
        }
        if (data.specialnet[specialnet_index].indirect_coordinate[1] == data.specialnet[specialnet_index].coordinate[1]) {
            LB[0] = data.specialnet[specialnet_index].coordinate[0];//x direction
            LB[1] = data.specialnet[specialnet_index].coordinate[1] - half_width;
            RT[0] = data.specialnet[specialnet_index].indirect_coordinate[0]; 
            RT[1] = data.specialnet[specialnet_index].coordinate[1] + half_width;
            TEXT[0] = data.specialnet[specialnet_index].coordinate[0];
            TEXT[1] = data.specialnet[specialnet_index].coordinate[1];
        }
        else if (data.specialnet[specialnet_index].indirect_coordinate[0] == data.specialnet[specialnet_index].coordinate[0]) {
            LB[0] = data.specialnet[specialnet_index].coordinate[0] - half_width;//y direction
            LB[1] = data.specialnet[specialnet_index].coordinate[1];
            RT[0] = data.specialnet[specialnet_index].coordinate[0] + half_width;
            RT[1] = data.specialnet[specialnet_index].indirect_coordinate[1];
            TEXT[0] = data.specialnet[specialnet_index].coordinate[0];
            TEXT[1] = data.specialnet[specialnet_index].indirect_coordinate[1];
        }
        output += "set object " + to_string(tag_counter) + " rect from ";
        output += to_string(LB[0]) + "," + to_string(LB[1]) + " to ";
        output += to_string(RT[0]) + "," + to_string(RT[1]) + " lw 1 fs solid fc rgb \"#" + color + "\"\n";
        output += "set label \"" + data.specialnet[specialnet_index].name + "\" at ";
        output += to_string(TEXT[0]) + "," + to_string(TEXT[1]) + " center\n";
        tag_counter++;
    }
    return output;
}
void plot_info::prt() {
    cout << command << endl;
}
void plot_info::write(string file_name) {
    os_info current;
    string output_path = current.path + "/" + file_name;
    ofstream outFile(output_path);
    if (!outFile) {
        cerr << "unable reach file!" << endl;
    }
    outFile << command;
    outFile.close();
    cout << output_path << ":end writing." << endl;
}