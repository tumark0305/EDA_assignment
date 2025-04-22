#ifndef TQDM_H
#define TQDM_H
#include <iostream>
#include <chrono>
#include <thread>

class tqdm {
private:
    int total;  // 總數量
    int current;  // 當前進度
    int bar_length;  // 進度條的長度
    std::string prefix;  // 進度條前綴

public:
    tqdm(int total, const std::string& prefix = "", int bar_length = 50);
    void update(int step = 1);
    void print_progress();
};
#endif
