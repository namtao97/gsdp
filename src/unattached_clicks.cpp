#include <iostream>
#include <fstream>

#include "unattached_clicks.hpp"
#include "serp_click.hpp"

unattached_clicks::unattached_clicks (const std::string date) {
	std::ifstream uclicks ("unattached_clicks-" + date);
	if (uclicks.is_open()) {
		std::string line;
		while (std::getline (uclicks, line)) {
			size_t tab_pos = line.find("\t");
			std::string url = line.substr (0, tab_pos);
			size_t tab_pos2 = line.find ("\t", tab_pos + 1);
			std::string doc_id = line.substr (tab_pos + 1, tab_pos2 - tab_pos - 1);
			int item_index = std::stoi (line.substr (tab_pos2 + 1));
			//insert (make_pair (make_pair(doc_id, item_index), url));
			operator[] (make_pair(doc_id, item_index)) = url;
		}
		uclicks.close();
	}
}

int unattached_clicks::dump_clicks (const std::string date, std::istream& in) {
	std::ofstream out;
	out.open ("unattached_clicks-" + date);
	std::string line;
	while (std::getline(in, line)) {
 		try {
			if (line == "<click_begin>") {
				serp_click click (in);
				out << click.get_url() << "\t" << click.get_doc_id() << "\t" <<  click.get_item_index() << "\n";
			}
		}
		catch  (const std::exception& e)  {
			std::cout << e.what() << " at " << line << "\n";
		}
	}
	out.close();
	return 0;
}

