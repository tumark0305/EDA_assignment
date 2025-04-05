#include "include.h"

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
        output += "set label \"" + data.component[component_index].macro_name + "\" at ";
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