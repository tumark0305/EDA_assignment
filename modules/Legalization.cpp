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
        new_coordinate[i] = other.new_coordinate[i];
    }

    history_coordinate = other.history_coordinate;
    tag = other.tag;

    // 深拷貝 sublock
    sublock = other.sublock;
}
// 深拷貝：建立新的子物件
BlockInfo& BlockInfo::operator=(const BlockInfo& other) {
    if (this == &other) return *this;
    local_reach_counter = other.local_reach_counter;

    for (int i = 0; i < 2; ++i) {
        coordinate[i] = other.coordinate[i];
        size[i] = other.size[i];
        step[i] = other.step[i];
        new_coordinate[i] = other.new_coordinate[i];
    }

    history_coordinate = other.history_coordinate;
    tag = other.tag;
    orientation = other.orientation;
    component_data = other.component_data;
    sublock = other.sublock;
    return *this;
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

    if (!sublock.empty()) {
        int min_x = INT_MAX, min_y = INT_MAX;
        for (const auto& s : sublock) {
            if (s.coordinate[0] < min_x) min_x = s.coordinate[0];
            if (s.coordinate[1] < min_y) min_y = s.coordinate[1];
        }

        int offset[2] = { coordinate[0] - min_x, coordinate[1] - min_y };

        for (auto& s : sublock) {
            s.coordinate[0] += offset[0];
            s.coordinate[1] += offset[1];
        }
    }
}

void BlockInfo::unprotect_move() {
    int hist[2] = { coordinate[0], coordinate[1] };
    history_coordinate.push_back({ hist[0], hist[1] });

    coordinate[0] += step[0];
    coordinate[1] += step[1];

    for (auto& s : sublock) {
        s.coordinate[0] += step[0];
        s.coordinate[1] += step[1];
    }
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
        s.coordinate[0] += diff[0];
        s.coordinate[1] += diff[1];
    }
}

float BlockInfo::global_distance() {
    float x2 = std::pow(history_coordinate[0][0] - coordinate[0], 2);
    float y2 = std::pow(history_coordinate[0][1] - coordinate[1], 2);
    return std::sqrt(x2 + y2);
}

void BlockInfo::unprotect_teleport() {
    int hist[2] = { coordinate[0], coordinate[1] };
    history_coordinate.push_back({ hist[0], hist[1] });

    int diff[2] = { new_coordinate[0] - coordinate[0], new_coordinate[1] - coordinate[1] };

    coordinate[0] = new_coordinate[0];
    coordinate[1] = new_coordinate[1];

    for (auto s : sublock) {
        s.coordinate[0] += diff[0];
        s.coordinate[1] += diff[1];
    }
}

void BlockInfo::cal_from_sublock() {
    tag = "combined";
    int min_x = INT_MAX, min_y = INT_MAX;
    int total_size[2] = { 0, 0 };
    int gv[2] = { 0, 0 };

    for (const auto& s : sublock) {
        if (s.coordinate[0] < min_x) min_x = s.coordinate[0];
        if (s.coordinate[1] < min_y) min_y = s.coordinate[1];
        total_size[0] += s.size[0];
        total_size[1] += s.size[1];

        if (!s.history_coordinate.empty()) {
            gv[0] += s.history_coordinate[0][0] - s.coordinate[0];
            gv[1] += s.history_coordinate[0][1] - s.coordinate[1];
        }
    }

    coordinate[0] = min_x;
    coordinate[1] = min_y;
    size[0] = total_size[0];
    size[1] = total_size[1];
}
unsigned int BlockInfo_col = 3;
unsigned int BlockInfo_row = 4;

bool operator==(const BlockInfo& a, const BlockInfo& b) {
    bool eqcoord = a.coordinate == b.coordinate;
    bool eqsize = a.size == b.size;
    return  eqcoord &&
        eqsize &&
        a.tag == b.tag &&
        a.orientation == b.orientation;
}


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
            std::array<int, 2> overlap_result = method.overlap(block_list[a], block_list[b]);
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

std::array<int, 2> legalization_method::overlap(BlockInfo blocka, BlockInfo blockb) {
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

void legalization_method::abacus() { // return output
    placed.clear();
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
            abacus_cal_cost(block_data_copy0[idx], option_row);
            all_cost.push_back(abacus_current_cost);
            all_condition.push_back(abacus_current_condition);
        }
        std::size_t min_index = 0;
        double min_cost = all_cost[0];
        for (std::size_t i = 1; i < all_cost.size(); ++i) {
            if (all_cost[i] < min_cost) {
                min_cost = all_cost[i];
                min_index = i;
            }
        }
        placed = all_condition[min_index];
    }
    output.clear();
    for (const auto& _block_orig : block_data_copy0) {
        for (const auto& _block_data : placed) {
            if (_block_orig.tag == _block_data.tag) {
                output.push_back(_block_data.coordinate);
                break;
            }
        }
    }
}

void legalization_method::abacus_cal_cost(BlockInfo input_block , int if_atrow) {//return abacus_cal_cost_output,abacus_cal_cost_placed_condition
    placed_mirror0.clear();
    placed_mirror0 = placed;
    BlockInfo  now_block = input_block;
    now_block.new_coordinate[1] = if_atrow;
    now_block.teleport();
    std::vector<BlockInfo> placed_condition;  // 設置放置條件
    std::vector<BlockInfo> placed_mirror1 = placed_mirror0;  // 複製 _placed_mirror0
    abacus_current_cost = 999.9;

    bool found_overlap = false;

    for (int touch_idx = 0; touch_idx < placed_mirror1.size(); touch_idx++) {
        std::array<int, 2> _overlap = overlap(now_block, placed_mirror1[touch_idx]); 
        if (_overlap[0] > 0 && _overlap[1] > 0) {
            cal_complex_loss(now_block);
            abacus_cal_cost_output = cal_complex_loss_output;
            abacus_cal_cost_placed_condition = cal_complex_loss_condition;
            found_overlap = true;
            break;
        }
    }
    if (!found_overlap) {
        // 計算距離
        float norm = std::sqrt(std::pow(now_block.history_coordinate[0][0] - now_block.coordinate[0], 2) +
            std::pow(now_block.history_coordinate[0][1] - now_block.coordinate[1], 2));
        cal_complex_loss_output = abacus_normal * norm;//alpha ==1 
        placed_mirror0.push_back(now_block);  // 添加到已放置區塊列表
        abacus_cal_cost_placed_condition = placed_mirror0;  // 更新放置條件
    }
}

BlockInfo legalization_method::combine_block(BlockInfo block_new, BlockInfo block_placed) {

}

void legalization_method::cal_complex_loss(BlockInfo now_block) {//return cal_complex_loss_output,cal_complex_loss_condition
    string save_tag = now_block.tag;
    BlockInfo new_block = now_block;
    new_block.tag = "current";
    std::vector< BlockInfo> effected_blocks;
    bool changed = true;
    std::vector<BlockInfo> placed_mirror1 = placed_mirror0; 
    while (changed) {
        changed = false;
        for (int i = 0; i < placed_mirror1.size(); i++) {
            std::array<int, 2> _overlap = overlap(new_block, placed_mirror1[i]);
            if (_overlap[0] > 0 && _overlap[1] > 0) {
                changed = true;
                effected_blocks.push_back(placed_mirror1[i]);
                placed_mirror0.erase(
                    std::remove(placed_mirror0.begin(), placed_mirror0.end(), placed_mirror1[i]),
                    placed_mirror0.end()
                );
                new_block = combine_block(new_block, placed_mirror1[i]); 
                int sum_vector = 0;
                for (int subidx = 0; subidx < new_block.sublock.size(); subidx++) {
                    sum_vector += new_block.sublock[subidx].history_coordinate[0][0] - new_block.sublock[subidx].coordinate[0];
                }
                new_block.step[0] = sum_vector / new_block.sublock.size();
                new_block.step[1] = 0;
                new_block.move();
            }
        }
    }
    placed_mirror0.push_back(new_block);
    float afterD = 0.0;
    float DL = 0.0;
    for (BlockInfo& _sublock : new_block.sublock) {
        if (_sublock.tag == "current") {
            DL += _sublock.global_distance();
            _sublock.tag = save_tag; // 假設 _save_tag 已經初始化
        }
        else {
            afterD += _sublock.global_distance(); 
        }
    }
    float beforeD = 0.0;
    for (BlockInfo& block : effected_blocks) {
        beforeD += block.global_distance();
    }
    cal_complex_loss_output = alpha * DL + afterD - beforeD;
    cal_complex_loss_condition = unpack(placed_mirror0);
    std::vector<bool> oversize;
    for (const BlockInfo& combined : placed_mirror0) {
        oversize.push_back(combined.size[0] > BlockInfo_row);
    }

    if (std::any_of(oversize.begin(), oversize.end(), [](bool v) { return v; })) {
        cal_complex_loss_output = std::numeric_limits<float>::infinity();
    }
}