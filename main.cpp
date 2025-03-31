#include <iostream>
#include "cuda_include.cuh"
//#include "cuda_include.cuh"

class test {
private:
	unsigned int mat_length = 0;
public:
	test(unsigned int length);
	void prt(double* mat);
	double* new_mat();
	double* matA = nullptr;
	double* matB = nullptr;
	double* matC = nullptr;
};

test::test(unsigned int length) {
    mat_length = length;
    matA = new_mat();
    matB = new_mat();
}
double* test::new_mat() {
    srand(time(0));
    double* matrix = new double[mat_length];
    for (int i = 0; i < mat_length; i++) {
        matrix[i] = static_cast<double>(std::rand()) / RAND_MAX; // 產生 0~1 之間的浮點數
    }
    return matrix;
}
void test::prt(double* mat) {
    string output = "";
    for (int i = 0; i < mat_length; i++) {
        output += to_string(mat[i]) + " ";
    }
    cout << output << endl;
}
using namespace std;
int main() {
	cout << "D" << endl;
	test sort(10);
	sort.prt(sort.matA);
	sort.prt(cuda_main(sort.matA));
	return 0;
}