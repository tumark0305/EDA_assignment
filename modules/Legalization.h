#ifndef LEGALIZATION_H
#define LEGALIZATION_H

#include "global_variable.h"
#include <sstream> 
#include <array>

struct loss_datapack_info {
    float quality = 9999.0;
    bool legal = false;
};

class BlockInfo {
private:
    static int reach_counter;
    unsigned int col = 3;
    unsigned int row = 4;
public:
    int local_reach_counter;
    std::array<int, 2> coordinate = { 0,0 };
    std::array<int, 2> size = { 0,0 };
    std::array<int, 2> step = { 0,0 };
    std::array<int, 2> global_vector = { 0,0 };
    std::array<int, 2> new_coordinate;
    std::vector<std::array<int, 2>> history_coordinate;
    std::string tag;
    std::string orientation;
    std::vector<BlockInfo*> sublock;
    BlockInfo(std::array<int, 2> _coordinate, std::array<int, 2> _size, std::string _orientation);
    BlockInfo(const BlockInfo& other); 
    BlockInfo& operator=(const BlockInfo& other);
    BlockInfo& operator*=(const BlockInfo& other);
    ~BlockInfo();

    std::array<int, 2> clip_coordinate();
    void move();
    void unprotect_move();
    void teleport();
    void unprotect_teleport();
    void cal_from_sublock();
};

class legalization_controller {
private:
    unsigned int col = 1;
    unsigned int row = 2;
    std::vector<loss_datapack_info> loss_datapack;
    unsigned int block_count = 0;
    std::vector<BlockInfo> block_list;
public:
    legalization_controller(data_info input_data);
    void forward();
    void loss(string method);
    float loss_quality_factor();
    std::array<int, 2> loss_overlap(BlockInfo blocka, BlockInfo blockb);
};

class legalization_method {
private:
    std::vector<BlockInfo> block_data;
public:
    legalization_method(std::vector<BlockInfo> input_data);
    void abacus(float abacus_alpha);
    std::vector<std::array<int, 2>> output;
};

#endif
