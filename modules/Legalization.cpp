#include "Legalization.h"

int BlockInfo::reach_counter = 0;

BlockInfo::BlockInfo(std::array<int, 2> _coordinate = { 0, 0 }, std::array<int, 2> _size = { 0, 0 },string _orientation= "N") {
    orientation = _orientation; 
    coordinate = _coordinate;
    size = _size;
    new_coordinate = _coordinate;
    local_reach_counter = ++reach_counter;
    history_coordinate.push_back(_coordinate);
    tag = std::to_string(local_reach_counter);
}

BlockInfo::BlockInfo(const BlockInfo& other) {
    // 複製 primitive members
    col = other.col;
    row = other.row;
    local_reach_counter = other.local_reach_counter;
    for (int i = 0; i < 2; ++i) {
        coordinate[i] = other.coordinate[i];
        size[i] = other.size[i];
        step[i] = other.step[i];
        global_vector[i] = other.global_vector[i];
        new_coordinate[i] = other.new_coordinate[i];
    }

    history_coordinate = other.history_coordinate;
    tag = other.tag;

    // 深拷貝 sublock
    sublock.clear();
    for (auto* blk : other.sublock) {
        if (blk) {
            sublock.push_back(new BlockInfo(*blk)); // 使用 copy constructor 再建一份
        }
        else {
            sublock.push_back(nullptr);
        }
    }
}

// 淺拷貝：只複製指標或結構，不動態分配新記憶體
BlockInfo& BlockInfo::operator=(const BlockInfo& other) {
    if (this == &other) return *this;

    col = other.col;
    row = other.row;
    local_reach_counter = other.local_reach_counter;

    for (int i = 0; i < 2; ++i) {
        coordinate[i] = other.coordinate[i];
        size[i] = other.size[i];
        step[i] = other.step[i];
        global_vector[i] = other.global_vector[i];
        new_coordinate[i] = other.new_coordinate[i];
    }

    history_coordinate = other.history_coordinate;
    tag = other.tag;
    sublock = other.sublock; // 淺拷貝：只複製指標容器，不創新物件

    return *this;
}

// 深拷貝：建立新的子物件
BlockInfo& BlockInfo::operator*=(const BlockInfo& other) {
    if (this == &other) return *this;

    col = other.col;
    row = other.row;
    local_reach_counter = other.local_reach_counter;

    for (int i = 0; i < 2; ++i) {
        coordinate[i] = other.coordinate[i];
        size[i] = other.size[i];
        step[i] = other.step[i];
        global_vector[i] = other.global_vector[i];
        new_coordinate[i] = other.new_coordinate[i];
    }

    history_coordinate = other.history_coordinate;
    tag = other.tag;

    // 先刪除原有 sublock
    for (auto* blk : sublock) delete blk;
    sublock.clear();

    // 深拷貝 sublock
    for (auto* blk : other.sublock) {
        sublock.push_back(blk ? new BlockInfo(*blk) : nullptr);
    }

    return *this;
}


BlockInfo::~BlockInfo() {
    for (auto* blk : sublock) {
        delete blk;
    }
    sublock.clear();
}


std::array<int, 2> BlockInfo::clip_coordinate() {
    std::array<int, 2> output;
    for (int i = 0; i < 2; i++) output[i] = coordinate[i];

    if (static_cast<unsigned int>(output[0] + size[0]) > row) output[0] = static_cast<int>(row - size[0]);
    if (output[0] < 0) output[0] = 0; 

    if (static_cast<unsigned int>(output[1] + size[1]) > col) output[1] = static_cast<int>(col - size[1]);
    if (output[1] < 0) output[1] = 0; 
    return output; 
}

void BlockInfo::move() {
    std::array<int, 2> hist = { coordinate[0], coordinate[1] };
    history_coordinate.push_back({ hist[0], hist[1] });

    coordinate[0] += step[0];
    coordinate[1] += step[1];

    std::array<int, 2> clipped = clip_coordinate();
    coordinate[0] = clipped[0];
    coordinate[1] = clipped[1];

    global_vector[0] = history_coordinate[0][0] - coordinate[0];
    global_vector[1] = history_coordinate[0][1] - coordinate[1];

    if (!sublock.empty()) {
        int min_x = INT_MAX, min_y = INT_MAX;
        for (auto s : sublock) {
            if (s->coordinate[0] < min_x) min_x = s->coordinate[0];
            if (s->coordinate[1] < min_y) min_y = s->coordinate[1];
        }
        int offset[2] = { coordinate[0] - min_x, coordinate[1] - min_y };
        for (auto s : sublock) {
            s->coordinate[0] += offset[0];
            s->coordinate[1] += offset[1];
        }
    }
}

void BlockInfo::unprotect_move() {
    int hist[2] = { coordinate[0], coordinate[1] };
    history_coordinate.push_back({ hist[0], hist[1] });

    coordinate[0] += step[0];
    coordinate[1] += step[1];

    for (auto s : sublock) {
        s->coordinate[0] += step[0];
        s->coordinate[1] += step[1];
    }

    global_vector[0] = history_coordinate[0][0] - coordinate[0];
    global_vector[1] = history_coordinate[0][1] - coordinate[1];
}

void BlockInfo::teleport() {
    int hist[2] = { coordinate[0], coordinate[1] };
    history_coordinate.push_back({ hist[0], hist[1] });

    coordinate[0] = new_coordinate[0];
    coordinate[1] = new_coordinate[1];

    std::array<int, 2> clipped = clip_coordinate();
    coordinate[0] = clipped[0];
    coordinate[1] = clipped[1];

    int diff[2] = { hist[0] - coordinate[0], hist[1] - coordinate[1] };

    for (auto s : sublock) {
        s->coordinate[0] += diff[0];
        s->coordinate[1] += diff[1];
    }

    global_vector[0] = history_coordinate[0][0] - coordinate[0];
    global_vector[1] = history_coordinate[0][1] - coordinate[1];
}

void BlockInfo::unprotect_teleport() {
    int hist[2] = { coordinate[0], coordinate[1] };
    history_coordinate.push_back({ hist[0], hist[1] });

    int diff[2] = { new_coordinate[0] - coordinate[0], new_coordinate[1] - coordinate[1] };

    coordinate[0] = new_coordinate[0];
    coordinate[1] = new_coordinate[1];

    for (auto s : sublock) {
        s->coordinate[0] += diff[0];
        s->coordinate[1] += diff[1];
    }

    global_vector[0] = history_coordinate[0][0] - coordinate[0];
    global_vector[1] = history_coordinate[0][1] - coordinate[1];
}

void BlockInfo::cal_from_sublock() {
    tag = "combined";
    int min_x = INT_MAX, min_y = INT_MAX;
    int total_size[2] = { 0, 0 };
    int gv[2] = { 0, 0 };

    for (auto s : sublock) {
        if (s->coordinate[0] < min_x) min_x = s->coordinate[0];
        if (s->coordinate[1] < min_y) min_y = s->coordinate[1];
        total_size[0] += s->size[0];
        total_size[1] += s->size[1];
        gv[0] += s->history_coordinate[0][0] - s->coordinate[0];
        gv[1] += s->history_coordinate[0][1] - s->coordinate[1];
    }

    coordinate[0] = min_x;
    coordinate[1] = min_y;
    size[0] = total_size[0];
    size[1] = total_size[1];
    global_vector[0] = gv[0];
    global_vector[1] = gv[1];
}

legalization_controller::legalization_controller(data_info input_data) {//translate data_info to block info
    std::cout << "hi" << std::endl;
}

legalization_method::legalization_method(std::vector<BlockInfo> input_data) {
    for (const auto& blk : input_data) {
        BlockInfo temp = blk; 
        temp *= blk; 
        block_data.push_back(temp); 
    }
}

void legalization_method::abacus(float abacus_alpha) {
    float x = abacus_alpha;
}

void legalization_controller::loss(string method) {
    int i = 1;
}

float legalization_controller::loss_quality_factor() {
    float i = 0.0;
    return i;
}

std::array<int, 2> legalization_controller::loss_overlap(BlockInfo blocka, BlockInfo blockb) {
    std::array<int, 2> output = { 0,0 };
    return output;
}

void legalization_controller::forward() {
    int i = 1;
}