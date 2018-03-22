#include <iostream>
#include <string>
#include "serp_item.hpp"

serp_item::serp_item (int index, std::istream& in) {
	item_index = index;
	std::string line;
	while (std::getline(in, line) && line.find_first_not_of (" \t-") != std::string::npos) {
		size_t pos_name = line.find_first_not_of (" \t");
		size_t pos_value = line.find (':');
		if (pos_name == std::string::npos || pos_value == std::string::npos)
			continue;
		std::string name =  line.substr (pos_name, pos_value - pos_name);
		std::string value = line.substr (pos_value + 2);
		if (name == "type")
			type = value;
		else if (name == "url")
			url = value;
		else if (name == "visible")
			visible_url = value;
		else if (name == "title")
			title = value;
		else if (name == "description")
			description = value;
		else if (name == "raw")
			raw = value;
	}
}

bool serp_item::has_date_snippet() const {
	size_t pos_date = description.find('-');
	if (pos_date == std::string::npos || pos_date > 22 || pos_date < 9)
		return false;
	if (description.substr(pos_date-5, 4).find_first_not_of("0123456789") == std::string::npos)
		return true;
	if ((description.substr(pos_date-4, 3) == "ago") 
		|| (description.substr(pos_date-9, 8) == "trước"))
		return true;
	return false;
}

bool serp_item::can_get_date() {
	size_t day_pos = description.find(' ');
	day = description.substr(0, day_pos);

	size_t temp_pos = description.find(' ', day_pos + 1);

	size_t month_pos = description.find(',', temp_pos + 1);
	month = description.substr(temp_pos + 1, month_pos - temp_pos - 1);

	size_t year_pos = description.find('-');
	year = description.substr(year_pos - 5, 4);

	if (day.size() < 2) day = "0" + day;
	if (month.size() < 2) month = "0" + month;

	bool valid_day = true;
	if (day.size() > 2) valid_day = false;
	for (int i = 0; i < day.size(); i++)
		if (day[i] < '0' || day[i] > '9')
			valid_day = false;

	bool valid_month = true;
	if (month.size() > 2) valid_month = false;
	for (int i = 0; i < month.size(); i++)
		if (month[i] < '0' || month[i] > '9')
			valid_month = false;

	return (valid_day && valid_month);
}

std::string serp_item::get_date() const {
	return (year + "-" + month + "-" + day);
}
