#include <iostream>
#include <string>
#include "serp_page.hpp"

serp_page::serp_page (std::istream& in) {
	std::string line;
	while (std::getline(in, line) && line != "<page_end>") {
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
		else if (name == "search_start")
			search_start = std::stoi (value);
		else if (name == "type")
			type = value;
		else if (name == "query")
			query = value;
		else if (name == "domain")
			domain = value;
		else if (name == "referer")
			referer = value;
		else if (name == "item_index") {
			serp_item sit (std::stoi (value), in);
			results.push_back (sit);
		}
	}
}

