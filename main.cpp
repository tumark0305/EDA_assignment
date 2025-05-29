#include "include.h"
string input_file_name = "pic60x60.in";
string output_file_name = "output.txt";
#define use_CUDA true
int main(int argc, char* argv[]) {
    if (get_parameters(argc, argv)) {
        return 1;
    }
    PIC_file file;
    if (file.read_fromfile(input_file_name)) return 1;
    PIC project(file , use_CUDA);
    project.direct_connect();
    cout << "loss: " << project.loss() << endl;
    file.read_fromdata(project.data);
    file.write_tofile(output_file_name);
    return 0;
}
