#ifndef __serp_item_hpp__
#define __serp_item_hpp__

#include <iostream>

class serp_item
{
private:
	int item_index;
	std::string type;
	std::string url;
	std::string visible_url;
	std::string title;
	std::string description;
	std::string raw;
	std::string day;
	std::string month;
	std::string year;
public:
	serp_item(int index, std::istream& in);
	virtual ~serp_item() {}
	int get_index() const { return item_index; }
	std::string get_type() const { return type; }
	std::string get_url() const { return url; }
	std::string get_title() const { return title; }
	std::string get_description() const { return description; }
	bool has_date_snippet() const;
	bool can_get_date();
	std::string get_date() const;
};

#endif // __serp_item_hpp__
