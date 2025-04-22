#include "tqdm.h"

tqdm::tqdm(int total, const std::string& prefix, int bar_length)
    : total(total), current(0), bar_length(bar_length), prefix(prefix) {
    print_progress();
}

void tqdm::update(int step) {
    current += step;
    if (current > total) current = total;  // 防止超過 total
    print_progress();
}

void tqdm::print_progress() {
    float progress = static_cast<float>(current) / total;  // 計算進度百分比
    int filled_length = static_cast<int>(bar_length * progress);  // 計算已填充的長度
    std::string bar = std::string(filled_length, '=') + std::string(bar_length - filled_length, ' ');  // 進度條的顯示

    // 打印進度條
    std::cout << "\r" << prefix << " [" << bar << "] " << int(progress * 100.0)
        << "% current:" << current << "/" << total;
    std::cout.flush();  // 強制刷新輸出

    if (current == total) {
        std::cout << std::endl;  // 結束後換行
    }
}