#ifndef LEGALIZATION_H
#define LEGALIZATION_H

#include "global_variable.h"
#include "tqdm.h"
#include "def_file.h"
#include <sstream> 
#include <array>
#include <cmath>
#include <algorithm>
#include <numeric>

struct loss_datapack_info {
    float quality = 9999.0;
    bool legal = false;
};
extern unsigned int BlockInfo_col;
extern unsigned int BlockInfo_row;

class BlockInfo {
private:
    static int reach_counter;
public:
    int local_reach_counter;
    component_info component_data;
    std::array<int, 2> coordinate = { 0,0 };
    std::array<int, 2> size = { 0,0 };
    std::array<int, 2> step = { 0,0 };
    std::array<int, 2> new_coordinate;
    std::array<int, 2> site_size;
    std::array<int, 2> global_placement_coordiante;
    std::vector<std::array<int, 2>> history_coordinate;
    std::string tag;
    std::string orientation;
    std::vector<BlockInfo> sublock;
    BlockInfo(component_info _original_data, std::array<int, 2> _size, std::array<int, 2> site_sizeincoord);
    BlockInfo(const BlockInfo& other); 
    BlockInfo& operator=(const BlockInfo& other);
    std::array<int, 2> clip_coordinate();
    void move();
    void unprotect_move();
    void teleport();
    void unprotect_teleport();
    void cal_from_sublock();
    float global_distance();
    std::array<int, 2> site_to_coordiante(std::array<int, 2> input_site_coordiante);
};
bool operator==(const BlockInfo& a, const BlockInfo& b);

class legalization_method {
private:
    std::vector< BlockInfo> placed; 
    std::vector<BlockInfo> placed_mirror0;
    std::vector<BlockInfo> block_data_copy0;
    double abacus_max_loss = 0.0;
    unsigned int block_count = 0;
    float abacus_current_cost = 999;
    std::vector< BlockInfo> abacus_current_condition;
    float abacus_cal_cost_output = 0.0;
    std::vector< BlockInfo> abacus_cal_cost_placed_condition;
    float cal_complex_loss_output = 0.0;
    std::vector< BlockInfo> cal_complex_loss_condition;
    void cal_complex_loss(BlockInfo now_block);
    void abacus_cal_cost(BlockInfo input_block, int if_atrow);
    BlockInfo combine_block(BlockInfo block_new , BlockInfo block_placed);
public:
    legalization_method() = default;
    std::array<int, 2> overlap(BlockInfo blocka, BlockInfo blockb);
    void load_data(std::vector<BlockInfo> input_data);
    void abacus();
    std::vector<std::array<int, 2>> output;
};

class legalization_controller {
private:
    unsigned int iter = 0;
    data_info input_data_pack_save;
    legalization_method method;
    std::array<int, 2> site_size = { 0,0 };
    std::vector<loss_datapack_info> loss_datapack;
    unsigned int block_count = 0;
    std::vector<BlockInfo> block_list;
    float quality_alpha;
    unsigned int cell_width;
    unsigned int cell_height;
public:
    legalization_controller(def_file input_file, float _quality_alpha, unsigned int _cell_width, unsigned int _cell_height);
    void forward(string _method);
    void loss(string _method);
    bool legal();
    float loss_quality_factor();
    data_info convert_data_pack(); 
};

#endif
