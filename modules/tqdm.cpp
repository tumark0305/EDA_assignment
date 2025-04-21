#include "tqdm.h"

tqdm::tqdm(int total, const std::string& prefix, int bar_length)
    : total(total), current(0), bar_length(bar_length), prefix(prefix) {
    print_progress();
}

void tqdm::update(int step) {
    current += step;
    if (current > total) current = total;  // ����W�L total
    print_progress();
}

void tqdm::print_progress() {
    float progress = static_cast<float>(current) / total;  // �p��i�צʤ���
    int filled_length = static_cast<int>(bar_length * progress);  // �p��w��R������
    std::string bar = std::string(filled_length, '=') + std::string(bar_length - filled_length, ' ');  // �i�ױ������

    // ���L�i�ױ�
    std::cout << "\r" << prefix << " [" << bar << "] " << int(progress * 100.0)
        << "% current:" << current << "/" << total;
    std::cout.flush();  // �j���s��X

    if (current == total) {
        std::cout << std::endl;  // �����ᴫ��
    }
}