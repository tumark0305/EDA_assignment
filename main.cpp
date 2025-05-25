#include "include.h"
string input_file_name = "pic5x5.in";
string output_file_name = "output.txt";
int main(int argc, char* argv[]) {
    //if (get_parameters(argc, argv)) {
    //    return 1;
    //}
    PIC_file file;
    if (file.read_fromfile(input_file_name)) return 1;
    
    return 0;
}
