#ifndef TQDM_H
#define TQDM_H
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip> 

class tqdm {
private:
    int total;  // �`�ƶq
    int current;  // ��e�i��
    int bar_length;  // �i�ױ�������
    std::string prefix;  // �i�ױ��e��
    std::chrono::steady_clock::time_point start_time; 
public:
    tqdm(int total, const std::string& prefix = "", int bar_length = 50);
    void update(int step = 1);
    void print_progress();
};
#endif
