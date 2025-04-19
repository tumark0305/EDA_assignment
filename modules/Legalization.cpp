#include "Legalization.h"
#define abacus_penalty 8.0
#define abacus_normal 1.0
int BlockInfo::reach_counter = 0;

BlockInfo::BlockInfo(std::array<int, 2> _coordinate, std::array<int, 2> _size ,string _orientation, component_info _original_data) {
    orientation = _orientation; 
    coordinate = _coordinate;
    size = _size;
    new_coordinate = _coordinate;
    local_reach_counter = ++reach_counter;
    history_coordinate.push_back(_coordinate);
    tag = std::to_string(local_reach_counter);

    component_data.inst_name = _original_data.inst_name;
    component_data.macro_name = _original_data.macro_name;
    component_data.place = _original_data.place;
    component_data.coordinate[0] = _original_data.coordinate[0]; //x,y
    component_data.coordinate[1] = _original_data.coordinate[1]; //x,y
    component_data.orientation = _original_data.orientation;//N=left bottom
}

BlockInfo::BlockInfo(const BlockInfo& other) {
    // 複製 primitive members
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

    if (static_cast<unsigned int>(output[0] + size[0]) > BlockInfo_row) output[0] = static_cast<int>(BlockInfo_row - size[0]);
    if (output[0] < 0) output[0] = 0; 

    if (static_cast<unsigned int>(output[1] + size[1]) > BlockInfo_col) output[1] = static_cast<int>(BlockInfo_col - size[1]);
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
unsigned int BlockInfo_col = 3;
unsigned int BlockInfo_row = 4;


legalization_controller::legalization_controller(data_info& input_data,float _quality_alpha, unsigned int _cell_width, unsigned int _cell_height) {//translate data_info to block info
    quality_alpha = _quality_alpha;
    cell_width = _cell_width;
    cell_height = _cell_height;
    int diearea_size[2] = { input_data.header.DIEAREA[1][0] - input_data.header.DIEAREA[0][0] ,input_data.header.DIEAREA[1][1] - input_data.header.DIEAREA[0][1] };
    BlockInfo_row = input_data.command[0].step[0];
    BlockInfo_col = input_data.command[0].num_size[1] * input_data.command.size();
    site_width = input_data.command[0].num_size[0];
    site_height = diearea_size[1] / BlockInfo_col;
    block_count = input_data.component.size();
    for (int i = 0; i < block_count; i++) {
        float coord[2];
        coord[0] = input_data.component[i].coordinate[0] / site_width;
        coord[1] = input_data.component[i].coordinate[1] / site_height;
        std::array<int, 2> block_coordinate = { std::round(coord[0]),std::round(coord[1])};
        std::array<int, 2> block_size = { _cell_width,_cell_height};
        string block_orientation = input_data.component[i].orientation;
        BlockInfo block(block_coordinate,block_size, block_orientation,input_data.component[i]);
        block_list.push_back(block);
        //std::cout << "register" << block.coordinate[0] << "," << block.coordinate[1] << std::endl;
    }
    method.load_data(block_list);
    std::cout << "hi" << BlockInfo_row <<"," << BlockInfo_col << std::endl;
}

//legalization_method::legalization_method() {}

void legalization_method::load_data(std::vector<BlockInfo> input_data) {
    for (const auto& blk : input_data) {
        BlockInfo temp = blk;
        temp *= blk;
        block_data_copy0.push_back(temp);
    }
    block_count = input_data.size();
}


bool legalization_controller::legal() {
    std::vector<int> all_x;
    std::vector<int> all_y;
    std::vector<bool> which_overlap;

    for (const auto& block : block_list) {
        all_x.push_back(block.coordinate[0]);
        all_x.push_back(block.coordinate[0] + block.size[0]);

        all_y.push_back(block.coordinate[1]);
        all_y.push_back(block.coordinate[1] + block.size[1]);
    }

    for (size_t a = 0; a < block_list.size(); ++a) {
        for (size_t b = a + 1; b < block_list.size(); ++b) {
            std::array<int, 2> overlap_result = loss_overlap(block_list[a], block_list[b]);
            bool is_overlapping = (overlap_result[0] > 0 && overlap_result[1] > 0);
            which_overlap.push_back(is_overlapping);
        }
    }

    bool L = *std::min_element(all_x.begin(), all_x.end()) >= 0;
    bool R = *std::max_element(all_x.begin(), all_x.end()) <= BlockInfo_row;
    bool B = *std::min_element(all_y.begin(), all_y.end()) >= 0;
    bool T = *std::max_element(all_y.begin(), all_y.end()) <= BlockInfo_col;

    bool no_overlap = std::none_of(which_overlap.begin(), which_overlap.end(), [](bool val) { return val; });

    return (L && R && B && T && no_overlap);
}


void legalization_controller::loss(string _method) {
    
    loss_datapack_info loss_data_pack;
    loss_data_pack.legal = legalization_controller::legal();
    
    loss_data_pack.quality = legalization_controller::loss_quality_factor();
    
    loss_datapack.push_back(loss_data_pack);
    
    if (_method == "spring") {
    }
    else if (_method == "abacus") {
        method.abacus();
    }
}

float legalization_controller::loss_quality_factor() {
    float sum = 0.0;
    float max_value=0.0;
    for (int i = 0; i < block_count; i++) {
        component_info block_component = block_list[i].component_data;
        int original_coord[2] = {block_component.coordinate[0] , block_component.coordinate[1]};
        int new_coord[2] = { block_list[i].coordinate[0] * cell_width ,block_list[i].coordinate[1] * cell_height };
        int global_vector[2] = { original_coord[0] - new_coord[0] ,original_coord[1] - new_coord[1] };

        float length = std::sqrt(std::pow(global_vector[0], 2) + std::pow(global_vector[1], 2));
        if (length > max_value)max_value = length;
        sum += length;
    }
    float output = sum/ block_count + quality_alpha * max_value;
    std::cout << "loss:" << output << std::endl;
    return output;
}

std::array<int, 2> legalization_controller::loss_overlap(BlockInfo blocka, BlockInfo blockb) {
    std::array<int, 2> x_range_a = { blocka.coordinate[0], blocka.coordinate[0] + blocka.size[0] };
    std::array<int, 2> y_range_a = { blocka.coordinate[1], blocka.coordinate[1] + blocka.size[1] };

    std::array<int, 2> x_range_b = { blockb.coordinate[0], blockb.coordinate[0] + blockb.size[0] };
    std::array<int, 2> y_range_b = { blockb.coordinate[1], blockb.coordinate[1] + blockb.size[1] };

    // 計算 x, y 方向的重疊長度
    int x_overlap = std::min(x_range_a[1], x_range_b[1]) - std::max(x_range_a[0], x_range_b[0]);
    int y_overlap = std::min(y_range_a[1], y_range_b[1]) - std::max(y_range_a[0], y_range_b[0]);

    std::array<int, 2> output = { x_overlap, y_overlap };
    return output;
}

void legalization_controller::forward(string _method) {
    
    if (_method == "spring") {
        for (int i = 0; i < block_count; i++) {
            block_list[i].move();
        }
        legalization_controller::loss(_method);
    }
    else if (_method == "abacus") {
        for (int i = 0; i < block_count; i++) {
            block_list[i].teleport();
        }
        legalization_controller::loss(_method);
    }
}

void legalization_method::abacus() { 
    std::vector< BlockInfo> placed;
    std::vector<int> all_x_coord;
    for (int i = 0; i < block_count; i++) {
        all_x_coord.push_back(block_data_copy0[i].coordinate[0]);
    }
    std::vector<int> sorted_indices(block_count);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(),
        [&](int a, int b) {
            return all_x_coord[a] < all_x_coord[b];
        });
    for (int idx : sorted_indices) {//X由小到大
        std::vector< float> all_cost;
        std::vector<std::vector< BlockInfo>> all_condition; 
        for (int option_row = 0; option_row < BlockInfo_col; option_row++) {

        }
        std::cout << "ordered:" << block_data_copy0[idx].coordinate[0] << std::endl;
    }
}

void legalization_method::abacus_cal_cost() {
    int i = 1;
}