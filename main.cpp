#include "include.h"
#include "cuda_include.cuh"
os current;
int main(int argc, char* argv[]) {
    cout << current.path << endl;
	test sort(10);
	sort.prt(sort.matA);
	sort.prt(cuda_main(sort.matA));
    if (get_parameters(argc, argv)) {
        return 1; 
    }
    input_file file(input_file_name);
    plot_info plot(file.data_pack, component_size);
    plot.write(output_file_name);
	return 0;
}