#include "PIC_file.h"

PIC_file::PIC_file() {
	os_info os;
	current_path = os.path;
}

bool PIC_file::read_fromfile(string input_file_name) {
	string input_path = current_path + "/" + input_file_name;
	string line;
	ifstream file(input_path);
	if (!file) {
		cerr << "cannot open file!" << input_path << endl;
		return 1;
	}

	while (getline(file, line)) {
		for (auto it = line.begin(); it != line.end(); ) {
			if (*it == '\r') {
				it = line.erase(it);
			}
			else {
				++it;

			}
		}
		//cout << " file line=" << line << endl;
		source_text += "\n";
		new_line(line);

	}
	file.close();
	return 0;
}

void PIC_file::new_line(string new_line_data) {
	vector<string> received_string;
	if (new_line_data.find("grid") != string::npos) {
		received_string = split(new_line_data);
		grid[0] = stoi(received_string[1]);
		grid[1] = stoi(received_string[2]);
	}
	else if (new_line_data.find("propagation") != string::npos) {
		received_string = split(new_line_data);
		propagation_loss = stoi(received_string[2]);
	}
	else if (new_line_data.find("crossing") != string::npos) {
		received_string = split(new_line_data);
		crossing_loss = stoi(received_string[2]);
	}
	else if (new_line_data.find("bending") != string::npos) {
		received_string = split(new_line_data);
		bending_loss = stoi(received_string[2]);
	}
	else if (new_line_data.find("num") != string::npos) {
		received_string = split(new_line_data);
		num_net = stoi(received_string[2]);
	}
	else if (!new_line_data.empty() && std::isdigit(new_line_data[0])) {
		net_info net;
		received_string = split(new_line_data);
		net.id = stoi(received_string[0]);
		net.pin_coordinate[0][0] = stoi(received_string[1]);
		net.pin_coordinate[0][1] = stoi(received_string[2]);
		net.pin_coordinate[1][0] = stoi(received_string[3]);
		net.pin_coordinate[1][1] = stoi(received_string[4]);
		data.push_back(net);
	}
}

vector<string> PIC_file::split(string input_word) {
	char key = ' ';
	vector<string> output;
	size_t space_pos1 = 0;
	size_t space_pos0 = 0;
	for (int _x = 0; _x < input_word.length(); _x++) {
		string current_output = "not find";
		space_pos1 = input_word.find(key, space_pos0 + 1);
		if (space_pos1 > input_word.length()) {
			space_pos1 = input_word.length();
		}
		current_output = input_word.substr(space_pos0, space_pos1 - space_pos0);
		output.push_back(current_output);
		space_pos0 = space_pos1;
		//cout << space_pos0 << endl;
		//cout << space_pos1 << endl;
		//cout << current_output << endl;
		if (space_pos1 == input_word.length()) {
			break;
		}
	}
	return output;
}

void PIC_file::write_tofile(string output_file_name) {
	std::ostringstream oss;
	string output_path = current_path + "/" + output_file_name;
	std::ofstream file(output_path);

	if (!file) {
		std::cerr << "cannot open file" << std::endl;
		return;
	}

	for (size_t i = 0; i < data.size(); ++i) {
		oss << data[i].id << " " << data[i].line_strip.size() << "\n";
		for (size_t j = 0; j < data[i].line_strip.size(); ++j) {
			const auto& seg = data[i].line_strip[j];
			oss << seg[0][0] << " " << seg[0][1] << " "
				<< seg[1][0] << " " << seg[1][1] << "\n";
		}
	}

	std::string print_preview = oss.str();  // Àx¦s¿é¥X¹wÄý
	file << print_preview;
	file.close();
	//cout << print_preview << endl;
}

bool PIC_file::read_fromdata(vector<net_info> input_data) {
	if (input_data.size() != 0) {
		data = input_data;
		return true;
	}
	else {
		return false;
	}
}