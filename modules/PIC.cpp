#include "PIC.h"

#define solve_limit 100

PIC::PIC(PIC_file input, bool use_cuda_bool) {
	header = input;
	data = input.data;
    use_cuda = use_cuda_bool;
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
    auto norm = [](const array<int, 2>& a, const array<int, 2>& b) -> double {
        return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
        };
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
    if (Dhor.empty()) {
        output[0] = Lver;
        output[1] = Rver;

        if (!Lver.empty() && norm(Lver[0], pin_location[0]) > 1.0) {
            output[0].insert(output[0].begin(), { Lver[0][0], pin_location[0][1] });
        }
        if (!Lver.empty() && norm(Lver.back(), pin_location[1]) > 1.0) {
            output[0].push_back({ Lver[0][0], pin_location[1][1] });
        }
        if (!Rver.empty() && norm(Rver[0], pin_location[0]) > 1.0) {
            output[1].insert(output[1].begin(), { Rver[0][0], pin_location[0][1] });
        }
        if (!Rver.empty() && norm(Rver.back(), pin_location[1]) > 1.0) {
            output[1].push_back({ Rver[0][0], pin_location[1][1] });
        }
    }
    if (Rver.empty()) {
        output[0] = Thor;
        output[1] = Dhor;

        if (!Thor.empty() && norm(Thor[0], pin_location[0]) > 1.0) {
            output[0].insert(output[0].begin(), { pin_location[0][0], Thor[0][1] });
        }
        if (!Thor.empty() && norm(Thor.back(), pin_location[1]) > 1.0) {
            output[0].push_back({ pin_location[1][0], Thor[0][1] });
        }
        if (!Dhor.empty() && norm(Dhor[0], pin_location[0]) > 1.0) {
            output[1].insert(output[1].begin(), { pin_location[0][0], Dhor[0][1] });
        }
        if (!Dhor.empty() && norm(Dhor.back(), pin_location[1]) > 1.0) {
            output[1].push_back({ pin_location[1][0], Dhor[0][1] });
        }
    }
    if(!(Dhor.empty() || Rver.empty())) {
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

vector<net_info> PIC::best_direct_connect_in_list(vector<net_info> pin_list) {
    vector<net_info> output = pin_list;
    vector<vector<vector<array<int, 2>>>> all_options;

    for (const auto& net : pin_list) {
        all_options.push_back(direct_connection(net.pin_coordinate));
    }

    size_t N = pin_list.size();
    int best_cross_count = INT_MAX;
    vector<vector<array<int, 2>>> best_routing(N);

    for (int selector = 0; selector < (1 << N); ++selector) {
        for (size_t i = 0; i < N; ++i) {
            pin_list[i].line_coordinate = all_options[i][(selector >> i) & 1];
        }

        int total_cross = 0;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = i + 1; j < N; ++j) {
                total_cross += cross_at(pin_list[i], pin_list[j]).size();
            }
        }

        if (total_cross < best_cross_count) {
            best_cross_count = total_cross;
            for (size_t i = 0; i < N; ++i) {
                best_routing[i] = pin_list[i].line_coordinate;
            }
        }
    }

    for (size_t i = 0; i < N; ++i) {
        output[i].line_coordinate = best_routing[i];
    }
    return output;
}

vector<net_info> PIC::from_smallest_direct_connect_in_list(vector<net_info> pin_list) {
    vector<net_info> output;
    vector<double> distances;

    for (const auto& net : pin_list) {
        double dx = net.pin_coordinate[0][0] - net.pin_coordinate[1][0];
        double dy = net.pin_coordinate[0][1] - net.pin_coordinate[1][1];
        distances.push_back(sqrt(dx * dx + dy * dy));
    }

    vector<size_t> sorted_indices(distances.size());
    iota(sorted_indices.begin(), sorted_indices.end(), 0);
    sort(sorted_indices.begin(), sorted_indices.end(), [&](size_t a, size_t b) {
        return distances[a] < distances[b];
        });

    for (size_t idx : sorted_indices) {
        net_info original_net = pin_list[idx];
        auto route_options = direct_connection(original_net.pin_coordinate);
        vector<int> cross_counts;

        for (const auto& option : route_options) {
            net_info test_net = original_net;
            test_net.line_coordinate = option;
            int cross_count = 0;
            for (const auto& placed : output) {
                cross_count += cross_at(test_net, placed).size();
            }
            cross_counts.push_back(cross_count);
        }

        int best_index = distance(cross_counts.begin(), min_element(cross_counts.begin(), cross_counts.end()));
        original_net.line_coordinate = route_options[best_index];
        output.push_back(original_net);
    }

    return output;
}

void PIC::direct_connect() {
    if (data.size() < solve_limit) { 
        if (use_cuda)  data = best_direct_connect_in_list_CUDA(data);
        else data = best_direct_connect_in_list(data);
    }
    else {
        vector<net_info> buffer = from_smallest_direct_connect_in_list(data);
        vector<int> crossing_table(buffer.size(), 0);

        for (size_t i = 0; i < buffer.size(); ++i) {
            for (size_t j = i + 1; j < buffer.size(); ++j) {
                int cross = cross_at(buffer[i], buffer[j]).size();
                crossing_table[i] += cross;
                crossing_table[j] += cross;
            }
        }

        vector<net_info> reroute_list, noroute_list;
        for (size_t i = 0; i < crossing_table.size(); ++i) {
            if (crossing_table[i] == 0) {
                noroute_list.push_back(buffer[i]);
            }
            else {
                reroute_list.push_back(buffer[i]);
            }
        }

        vector<net_info> rerouted;
        if (use_cuda)  rerouted = best_direct_connect_in_list_CUDA(reroute_list);
        else rerouted = best_direct_connect_in_list(reroute_list);

        data = rerouted;
        data.insert(data.end(), noroute_list.begin(), noroute_list.end());
    }
}

vector<net_info> PIC::best_direct_connect_in_list_CUDA(vector<net_info> pin_list) {
    vector<net_info> output = pin_list;
    vector<vector<vector<array<int, 2>>>> all_options;

    for (const auto& net : pin_list) {
        all_options.push_back(direct_connection(net.pin_coordinate));
    }

    size_t N = pin_list.size();
    vector<int> option_flat;
    vector<int> option_lengths;
    vector<int> option_offsets;
    int max_path_len = 0;

    for (const auto& options : all_options) {
        for (const auto& path : options) {
            option_offsets.push_back(option_flat.size());
            option_lengths.push_back((path.size() - 1) * 4);
            max_path_len = max(max_path_len, (int)(path.size() - 1) * 4);

            for (size_t i = 1; i < path.size(); ++i) {
                option_flat.push_back(path[i - 1][0]); // x1
                option_flat.push_back(path[i - 1][1]); // y1
                option_flat.push_back(path[i][0]);     // x2
                option_flat.push_back(path[i][1]);     // y2
            }
        }
    }

    vector<int> cross_counts = evaluate_crossing_cuda(
        option_flat, option_lengths, option_offsets,
        static_cast<int>(N), max_path_len
    );

    int best_selector = min_element(cross_counts.begin(), cross_counts.end()) - cross_counts.begin();

    for (size_t i = 0; i < N; ++i) {
        int opt = (best_selector >> i) & 1;
        output[i].line_coordinate = all_options[i][opt];
    }
    return output;
}