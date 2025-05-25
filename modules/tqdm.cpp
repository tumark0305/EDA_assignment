#include "tqdm.h"

tqdm::tqdm(int total, const std::string& prefix, int bar_length)
    : total(total), current(0), bar_length(bar_length), prefix(prefix) {
    start_time = std::chrono::steady_clock::now();
    print_progress();
}

void tqdm::update(int step) {
    current += step;
    if (current > total) current = total;  // 防止超過 total
    print_progress();
}

void tqdm::print_progress() {
    using namespace std::chrono;

    // 計算進度
    float progress = static_cast<float>(current) / total;
    int filled_length = static_cast<int>(bar_length * progress);
    std::string bar = std::string(filled_length, '=') + std::string(bar_length - filled_length, ' ');

    // 計算速度
    auto now = steady_clock::now();
    auto elapsed = duration_cast<duration<double>>(now - start_time).count();  // 秒數
    double speed = (elapsed > 0.0) ? current / elapsed : 0.0;

    std::ostringstream rate_ss;
    if (speed >= 1.0) {
        rate_ss << std::fixed << std::setprecision(2) << speed << " it/s";
    }
    else {
        double sec_per_it = (current > 0) ? elapsed / current : 0.0;
        rate_ss << std::fixed << std::setprecision(2) << sec_per_it << " s/it";
    }

    // 輸出進度條
    std::cout << "\r" << prefix << " [" << bar << "] "
        << int(progress * 100.0) << "% "
        << "current:" << current << "/" << total
        << " - " << rate_ss.str();
    std::cout.flush();

    if (current == total) {
        std::cout << std::endl;  // 換行收尾
    }
}