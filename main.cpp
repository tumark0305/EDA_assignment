#include "include.h"
#define output_gp true
string input_file_name = "super1.def";
string output_file_name = "output.gp";
unsigned int cell_width = 2;
unsigned int cell_height = 1;
float quality_alpha = 1.0;
unsigned int component_size[2] = {0,0};
int main(int argc, char* argv[]) {
    if (get_parameters(argc, argv)) {
        return 1;
    }
    def_file file;
    if (file.read_fromfile(input_file_name)) return 1;
    
    legalization_controller legalization(file, quality_alpha, cell_width,cell_height);
    cout << "training :" << endl;
    legalization.forward("abacus");//ep0  init
    legalization.forward("abacus");//ep1 will output first result
    legalization.forward("abacus");
    data_info data_pack_output = legalization.convert_data_pack();
    cout << "writing" << endl;
    component_size[0] = file.site_size[0] * cell_width;
    component_size[1] = file.site_size[1] * cell_height;
    plot_info plot(data_pack_output, component_size);
    if (output_gp)    plot.write("output.gp");
    file.read_fromdata(data_pack_output);
    file.write(output_file_name);
    
    return 0;
}
