#include "include.h"
unsigned int CS_width = 0;
unsigned int CS_height = 0;
string input_file_name = "CS8x8.def";
string output_file_name = "output.gp";
int main() {
    input_file input_buffer(input_file_name);
    cout << input_buffer.specialnet[0].name << endl;
    return 0;
}
