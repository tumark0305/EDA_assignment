#ifndef TQDM_H
#define TQDM_H
#include <iostream>
#include <chrono>
#include <thread>

class tqdm {
private:
    int total;  // �`�ƶq
    int current;  // ��e�i��
    int bar_length;  // �i�ױ�������
    std::string prefix;  // �i�ױ��e��

public:
    tqdm(int total, const std::string& prefix = "", int bar_length = 50);
    void update(int step = 1);
    void print_progress();
};
#endif
