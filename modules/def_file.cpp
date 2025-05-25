#include "def_file.h"
data_info def_file::data_pack; //all acessable
def_file::def_file() {
	os_info current;
	path = current.path;
}
bool def_file::read_fromfile(string file_name) {
	string input_path = path + "/" + file_name;
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
		input_line(line);
	}
	file.close();
	data_pack.header = header;
	data_pack.command = command;
	data_pack.component = component;
	data_pack.specialnet = specialnet;
	site_size[0] = command[0].step[0];
	site_size[1] = command[1].coordinate[1] - command[0].coordinate[1];
	return 0;
}
void def_file::input_line(string new_line) {
	
	if (new_line.find("ROW") != string::npos) {
		put_command(new_line);
	}
	else if (line_tag == "OFF" && new_line.find("VERSION") != string::npos) {
		line_tag = "head";
		head_text += "\n" + new_line;
		put_header(new_line);
	}
	else if (line_tag == "head") {
		if (new_line.find("COMPONENTS") != string::npos) {
			line_tag = "components";
			components_text += "\n" + new_line;
			components_len = stoi(get_bykeyword(new_line, ' '));
		}
		else {
			head_text += "\n" + new_line;
			
			put_header(new_line);
		}
	}
	else if (line_tag == "components") {
		components_text += "\n" + new_line;
		if (new_line.find(";") != string::npos) {
			put_component(buffer + new_line);
		}
		else {
			buffer = new_line;
		}
		if (new_line == "END COMPONENTS") {
			line_tag = "OFF";
		}
	}
	else if (line_tag == "OFF" && new_line.find("SPECIALNETS") != string::npos) {
		line_tag = "specialnets";
		specialnets_text += "\n" + new_line;
		specialnets_len = stoi(get_bykeyword(new_line, ' '));
	}
	else if (line_tag == "specialnets") {
		specialnets_text += "\n" + new_line;
		if (new_line.find(";") != string::npos) {
			put_specialnet(buffer + new_line);
		}
		else {
			buffer = new_line;
		}
		if (new_line == "END SPECIALNETS") {
			line_tag = "OFF";
		}
	}
}
void def_file::put_header(string new_line) {
	if (new_line.find("VERSION") != string::npos) {
		header.VERSION = get_bykeyword(new_line, ' ');
	}
	else if (new_line.find("DIVIDERCHAR") != string::npos) {
		header.DIVIDERCHAR = get_bykeyword(new_line, '"');
	}
	else if (new_line.find("BUSBITCHARS") != string::npos) {
		header.BUSBITCHARS = get_bykeyword(new_line, '"');
	}
	else if (new_line.find("DESIGN") != string::npos) {
		size_t space_pos1 = new_line.find(";");
		size_t space_pos0 = new_line.find("N");
		if (space_pos0 > new_line.length() || space_pos1 > new_line.length()) {
			ostringstream error_message;
			error_message << "space_pos0 = " << space_pos0 << "\nspace_pos1 = " << space_pos1;
			throw std::runtime_error(error_message.str());
		}
		header.DESIGN = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
	}
	else if (new_line.find("UNITS DISTANCE MICRONS") != string::npos) {
		
		header.UNITS_DISTANCE_MICRONS = stoi(get_bykeyword(new_line, ' '));
	}
	else if (new_line == "PROPERTYDEFINITIONS") {
		PROPERTYDEFINITIONS = true;
	}
	else if (new_line == "END PROPERTYDEFINITIONS") {
		PROPERTYDEFINITIONS = false;
	}
	else if (PROPERTYDEFINITIONS) {
		
		size_t space_pos1 = new_line.rfind(" ");
		size_t space_pos0 = new_line.find(" ");
		if (space_pos0 > new_line.length() || space_pos1 > new_line.length()) {
			ostringstream error_message;
			error_message << "space_pos0 = " << space_pos0 << "\nspace_pos1 = " << space_pos1;
			throw std::runtime_error(error_message.str());
		}
		header.PROPERTYDEFINITIONS = new_line.substr(space_pos0 + 2, space_pos1 - space_pos0 - 2);
	}
	else if (new_line.find("DIEAREA") != string::npos) {
		
		size_t space_pos1 = new_line.rfind(" ");
		size_t space_pos0 = new_line.find(" ");
		if (space_pos0 > new_line.length() || space_pos1 > new_line.length()) {
			ostringstream error_message;
			error_message << "space_pos0 = " << space_pos0 << "\nspace_pos1 = " << space_pos1;
			throw std::runtime_error(error_message.str());
		}
		string str_cord = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
		space_pos1 = 0;
		for (unsigned int x = 0; x < 2; x++) {
			for (unsigned int y = 0; y < 2; y++) {
				space_pos0 = str_cord.find(" ", space_pos1);
				space_pos1 = str_cord.find(" ", space_pos0 + 1);
				header.DIEAREA[x][y] = stoi(str_cord.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
			}
			space_pos1 += 3;
		}
	}
	//return header;;
}
void def_file::put_command(string new_line) {
	command_info _info;
	string  _coordinate_text[2], _num_size_text[2], _step_text[2];
	size_t space_pos1 = 0;
	size_t space_pos0 = 0;
	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_info.row_name = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_info.site_name = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_coordinate_text[0] = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_coordinate_text[1] = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_info.orientation = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_num_size_text[0] = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_num_size_text[1] = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_step_text[0] = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_step_text[1] = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	_info.coordinate[0] = stoi(_coordinate_text[0]);
	_info.coordinate[1] = stoi(_coordinate_text[1]);
	_info.num_size[0] = stoi(_num_size_text[0]);
	_info.num_size[1] = stoi(_num_size_text[1]);
	_info.step[0] = stoi(_step_text[0]);
	_info.step[1] = stoi(_step_text[1]);
	command.push_back(_info);
}
void def_file::put_component(string new_line) {
	component_info _info;
	string  _coordinate_text;
	size_t space_pos1 = 0;
	size_t space_pos0 = 0;
	space_pos0 = new_line.find("-", 0);
	space_pos1 = new_line.find(" ", space_pos0 + 2);
	_info.inst_name = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find("+", space_pos0 + 1);
	_info.macro_name = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
	space_pos0 = new_line.find("+", space_pos1);
	space_pos1 = new_line.find("(", space_pos0 + 1);
	_info.place = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
	space_pos0 = new_line.find("(", space_pos1);
	space_pos1 = new_line.find(")", space_pos0 + 1);
	_coordinate_text = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_info.orientation = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
	space_pos1 = 0;
	space_pos0 = _coordinate_text.find(" ", space_pos1);
	space_pos1 = _coordinate_text.find(" ", space_pos0 + 1);

	_info.coordinate[0] = stoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
	space_pos0 = _coordinate_text.find(" ", space_pos1);
	space_pos1 = _coordinate_text.find(" ", space_pos0 + 1);

	_info.coordinate[1] = stoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
	component.push_back(_info);
}
void def_file::put_specialnet(string new_line) {
	specialnet_info _info;
	string  _coordinate_text, _indirect_coordinate_text;
	size_t space_pos1 = 0;
	size_t space_pos0 = 0;
	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_info.name = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1 + 2);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_info.rout = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_info.layer = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find(" ", space_pos1);
	space_pos1 = new_line.find(" ", space_pos0 + 1);
	_info.width = stoi(new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));

	space_pos0 = new_line.find("(", space_pos1);
	space_pos1 = new_line.find(")", space_pos0 + 1);
	_coordinate_text = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);

	space_pos0 = new_line.find("(", space_pos1);
	space_pos1 = new_line.find(")", space_pos0 + 1);
	_indirect_coordinate_text = new_line.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
	space_pos1 = 0;
	for (int x0 = 0; x0 < 2; x0++) {
		space_pos0 = _coordinate_text.find(" ", space_pos1);
		space_pos1 = _coordinate_text.find(" ", space_pos0 + 1);
		_info.coordinate[x0] = stoi(_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1));
	}
	space_pos1 = 0;
	for (int x1 = 0; x1 < 2; x1++) {
		space_pos0 = _indirect_coordinate_text.find(" ", space_pos1);
		space_pos1 = _indirect_coordinate_text.find(" ", space_pos0 + 1);
		string _indirect_coordinate = _indirect_coordinate_text.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
		if (_indirect_coordinate == "*") {
			_info.indirect_coordinate[x1] = _info.coordinate[x1];
		}
		else {
			_info.indirect_coordinate[x1] = stoi(_indirect_coordinate);
		}
	}
	specialnet.push_back(_info);
}
string def_file::get_bykeyword(string word, char key) {
	string output = "not find";
	size_t space_pos1 = word.rfind(key);
	size_t space_pos0 = word.rfind(key, space_pos1 - 1);
	if (space_pos0 > word.length() || space_pos1 > word.length()) {
		ostringstream error_message;
		error_message << "space_pos0 = " << space_pos0 << "\nspace_pos1 = " << space_pos1;
		throw std::runtime_error(error_message.str());
	}
	output = word.substr(space_pos0 + 1, space_pos1 - space_pos0 - 1);
	//cout << space_pos0 << endl;
	//cout << space_pos1 << endl;
	//cout << output << endl;
	return output;
}

void def_file::read_fromdata(data_info data_input) {
	data_pack = data_input;
}

void def_file::write(string file_name) {
	line_counter = 0;
	string output_path = path + "/" + file_name;
	std::ofstream file(output_path);  // 開啟或建立 output.txt
	if (!file) {
		std::cerr << "cannot open file" << std::endl;
	}

	file << "VERSION " + data_pack.header.VERSION + " ;" << "\n";
	file << "DIVIDERCHAR \"" + data_pack.header.DIVIDERCHAR + "\" ;" << "\n";
	file << "BUSBITCHARS \""+ data_pack.header .BUSBITCHARS +"\" ;" << "\n";
	file << "DESIGN "+ data_pack.header.DESIGN +" ;" << "\n";
	file << "UNITS DISTANCE MICRONS "+ to_string(data_pack.header.UNITS_DISTANCE_MICRONS) + " ;" << "\n\n";
	file << "DIEAREA ( "+ to_string(data_pack.header.DIEAREA[0][0]) + " " + to_string(data_pack.header.DIEAREA[0][1]) + " ) ( " + to_string(data_pack.header.DIEAREA[1][0]) + " " + to_string(data_pack.header.DIEAREA[1][1]) + " ) ;" << "\n\n";
	for (int i = 0; i < data_pack.command.size(); i++) {
		command_info row = data_pack.command[i];
		file << "ROW "+ row.row_name+" " + row.site_name + " " + to_string(row.coordinate[0]) + " " + to_string(row.coordinate[1]) + " " + row.orientation + " DO " + to_string(row.num_size[0]) + " BY " + to_string(row.num_size[1]) + " STEP " + to_string(row.step[0]) + " " + to_string(row.step[1]) + " ;" << "\n";
	}
	file << "\nCOMPONENTS "+to_string(data_pack.component.size()) + " ;" << "\n";
	for (int i = 0; i < data_pack.component.size(); i++) {
		component_info row = data_pack.component[i];
		int coord[2] = { row.coordinate[0]  , row.coordinate[1] };
		//if (row.orientation == "FS") coord[1] += site_size[1];
		file << "- "+ row .inst_name +" " + row.macro_name + "" << "\n";
		file << "  + " + row.place + " ( " + to_string(coord[0]) + " " + to_string(coord[1]) + " ) " + row.orientation + " ;" << "\n";
	}
	file << "END COMPONENTS" << "\n\n";
	file << "END DESIGN" << std::endl;



	file.close();
}