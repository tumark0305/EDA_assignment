#include "include.h"

int main(int argc, char* argv[]) {
    if (get_parameters(argc, argv)) {
        return 1;
    }
    input_file file(input_file_name);
    plot_info plot(file.data_pack, component_size);
    //plot.prt();
    plot.write(output_file_name);
    return 0;
}
