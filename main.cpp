#include "include.h"

string input_file_name = "case1.def";
string output_file_name = "output.gp";
unsigned int cell_width = 1;
unsigned int cell_height = 1;
float quality_alpha = 1.0;
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
    legalization.forward("abacus");
    cout << "end" << endl;
    
    return 0;
}
