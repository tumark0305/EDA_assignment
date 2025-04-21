#include "include.h"

string input_file_name = "case1.def";
string output_file_name = "output.gp";
unsigned int cell_width = 40;
unsigned int cell_height = 1;
float quality_alpha = 1.0;
unsigned int component_size[2] = { 50 * cell_width ,2000 * cell_height };
int main(int argc, char* argv[]) {
    /*if (get_parameters(argc, argv)) {
        return 1;
    }
    plot_info plot(file.data_pack, component_size);
    plot.write(output_file_name);
    */
    def_file file;
    file.read_fromfile(input_file_name);
    legalization_controller legalization(file.data_pack, quality_alpha, cell_width,cell_height);
    legalization.forward("abacus");
    data_info data_pack_output = legalization.convert_data_pack();
    cout << "writing" << endl;
    plot_info plot(data_pack_output , component_size);
    plot.write(output_file_name);
    file.read_fromdata(data_pack_output);
    file.write("test.txt");
    
    return 0;
}
