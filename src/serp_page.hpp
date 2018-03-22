#ifndef __serp_page_hpp__
#define __serp_page_hpp__

#include <iostream>
#include <vector>

#include "serp_item.hpp"
#include "unattached_clicks.hpp"

class serp_page
{
private:
	std::string doc_id;
	std::string type;
	std::string query;
	std::string domain;
	std::string referer;
	int search_start;
	std::string timestamp;
	std::vector <serp_item> results;
public:
	serp_page(std::istream& in);
	virtual ~serp_page() {}
	int get_search_start() const { return search_start; }
	std::string get_query() const { return query; }
	std::string get_doc_id() const { return doc_id; }
	std::string get_timestamp() const { return timestamp; }
	std::string get_domain() const { return domain; }
	std::vector <serp_item>& get_results() { return results; }
};

#endif // __serp_page_hpp__

