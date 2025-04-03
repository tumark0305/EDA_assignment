#include "include.h"
string input_file_name = "CS12x12.def";
string output_file_name = "output.gp";
unsigned int component_size[2] = { 7100, 6600 };
int main(int argc, char* argv[]) {
    if (get_parameters(argc, argv)) {
        return 1;
    }
    input_file_final file(input_file_name);
    plot_info plot(file.data_pack, component_size);
    plot.write(output_file_name);
    return 0;
}
