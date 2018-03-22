#include <iostream>
#include <string>
#include "serp_click.hpp"

serp_click::serp_click (std::istream& in) {
	std::string line;
	while (std::getline(in, line) && line != "<click_end>") {
		size_t pos_name = line.find_first_not_of (" \t");
		size_t pos_value = line.find (':');
		if (pos_name == std::string::npos || pos_value == std::string::npos)
			continue;
		std::string name =  line.substr (pos_name, pos_value - pos_name);
		std::string value = line.substr (pos_value + 2);
		if (name == "timestamp")
			timestamp = value;
		else if (name == "doc_id")
			doc_id = value;
		else if (name == "item_index")
			item_index = std::stoi (value);
		else if (name == "url")
			url = value;
		else if (name == "redirect")
			 redirect = value;
		else if (name == "request_id")
			 request_id = value;
	}
}
