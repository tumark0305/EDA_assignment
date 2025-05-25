#include "PIC.h"

PIC::PIC(PIC_file input) {
	header = input;
	data = input.data;
}

vector<array<int, 2>> PIC::cross_at(const net_info& pina, const net_info& pinb) {
    set<array<int, 2>> neta_area;
    set<array<int, 2>> netb_area;

    for (const auto& pt : pina.line_coordinate) {
        neta_area.insert(pt);
    }
    for (const auto& pt : pina.pin_coordinate) {
        neta_area.insert(pt);
    }
    for (const auto& pt : pinb.line_coordinate) {
        netb_area.insert(pt);
    }
    for (const auto& pt : pinb.pin_coordinate) {
        netb_area.insert(pt);
    }

    vector<array<int, 2>> output;
    for (const auto& pt : neta_area) {
        if (netb_area.count(pt)) {
            output.push_back(pt);
        }
    }
    return output;
}

vector<vector<array<int, 2>>> PIC::direct_connection(const array<array<int, 2>, 2>& pin_location) {
    vector<vector<array<int, 2>>> output(2);

    int x0 = pin_location[0][0];
    int y0 = pin_location[0][1];
    int x1 = pin_location[1][0];
    int y1 = pin_location[1][1];

    int xmin = min(x0, x1);
    int xmax = max(x0, x1);
    int ymin = min(y0, y1);
    int ymax = max(y0, y1);

    array<int, 2> LD_point = { xmin, ymin };
    array<int, 2> RT_point = { xmax, ymax };
    array<int, 2> LT_point = { xmin, ymax };
    array<int, 2> RD_point = { xmax, ymin };

    vector<array<int, 2>> Dhor, Rver, Thor, Lver;

    for (int x = xmin + 1; x < xmax; ++x) {
        Dhor.push_back({ x, ymin });
        Thor.push_back({ x, ymax });
    }
    for (int y = ymin + 1; y < ymax; ++y) {
        Rver.push_back({ xmax, y });
        Lver.push_back({ xmin, y });
    }

    // 若起點在右邊，反轉水平方向；在上方則反轉垂直方向
    if (x0 > x1) {
        reverse(Dhor.begin(), Dhor.end());
        reverse(Thor.begin(), Thor.end());
    }
    if (y0 > y1) {
        reverse(Rver.begin(), Rver.end());
        reverse(Lver.begin(), Lver.end());
    }

    if (Dhor.empty() || Rver.empty()) {
        output[0] = Lver;
        output[0].insert(output[0].end(), Thor.begin(), Thor.end());

        output[1] = Dhor;
        output[1].insert(output[1].end(), Rver.begin(), Rver.end());
    }
    else {
        if (x0 == LD_point[0] && y0 == LD_point[1]) {
            output[0] = Lver;
            output[0].push_back(LT_point);
            output[0].insert(output[0].end(), Thor.begin(), Thor.end());

            output[1] = Dhor;
            output[1].push_back(RD_point);
            output[1].insert(output[1].end(), Rver.begin(), Rver.end());
        }
        else if (x0 == LT_point[0] && y0 == LT_point[1]) {
            output[0] = Lver;
            output[0].push_back(LD_point);
            output[0].insert(output[0].end(), Dhor.begin(), Dhor.end());

            output[1] = Thor;
            output[1].push_back(RT_point);
            output[1].insert(output[1].end(), Rver.begin(), Rver.end());
        }
        else if (x0 == RD_point[0] && y0 == RD_point[1]) {
            output[0] = Rver;
            output[0].push_back(RT_point);
            output[0].insert(output[0].end(), Thor.begin(), Thor.end());

            output[1] = Dhor;
            output[1].push_back(LD_point);
            output[1].insert(output[1].end(), Lver.begin(), Lver.end());
        }
        else if (x0 == RT_point[0] && y0 == RT_point[1]) {
            output[0] = Rver;
            output[0].push_back(RD_point);
            output[0].insert(output[0].end(), Dhor.begin(), Dhor.end());

            output[1] = Thor;
            output[1].push_back(LT_point);
            output[1].insert(output[1].end(), Lver.begin(), Lver.end());
        }
    }

    return output;
}

int PIC::loss_cal_cross() {
    int output = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        const auto& pin_a = data[i];
        for (size_t j = i + 1; j < data.size(); ++j) {
            const auto& pin_b = data[j];
            for (const auto& path_a : pin_a.line_strip) {
                for (const auto& path_b : pin_b.line_strip) {
                    bool eq_start = (path_a[0][0] == path_b[0][0]) && (path_a[0][1] == path_b[0][1]);
                    bool eq_end = (path_a[1][0] == path_b[1][0]) && (path_a[1][1] == path_b[1][1]);
                    if (eq_end) {
                        ++output;
                    }
                }
            }
        }
    }
    return output;
}

int PIC::loss_cal_bend() {
    int output = 0;
    for (const auto& pin : data) {
        string current_direction = "N";
        for (const auto& path : pin.line_strip) {
            string next_direction = "N";
            if (path[0][0] == path[1][0]) {
                next_direction = "Y";
            }
            else if (path[0][1] == path[1][1]) {
                next_direction = "X";
            }
            else {
                cerr << "Invalid line segment detected" << endl;
            }
            if (current_direction != "N" && current_direction != next_direction) {
                ++output;
            }
            current_direction = next_direction;
        }
    }
    return output;
}

int PIC::loss_cal_length() {
    int output = 0;
    for (const auto& net : data) {
        output += static_cast<int>(net.line_strip.size());
    }
    return output;
}

void PIC::convert_toline() {
    for (auto& pin : data) {
        pin.line_strip.clear();

        array<int, 2> current_coordinate = pin.pin_coordinate[0];
        array<int, 2> end_coordinate = pin.pin_coordinate[1];

        vector<array<int, 2>> connect_in = pin.line_coordinate;
        if (!(abs(connect_in[0][0] - current_coordinate[0]) <= 1 &&
            abs(connect_in[0][1] - current_coordinate[1]) <= 1)) {
            current_coordinate = pin.pin_coordinate[1];
            end_coordinate = pin.pin_coordinate[0];
        }
        connect_in.push_back(end_coordinate);

        for (const auto& next_coordinate : connect_in) {
            array<array<int, 2>, 2> line = { current_coordinate, next_coordinate };
            pin.line_strip.push_back(line);
            current_coordinate = next_coordinate;
        }
    }
}

double PIC::loss() {
    convert_toline();
    int wire_length = loss_cal_length();
    int cross = loss_cal_cross();
    int bend = loss_cal_bend();
    double total_loss = wire_length * header.propagation_loss +
        cross * header.crossing_loss +
        bend * header.bending_loss;
    return total_loss;
}