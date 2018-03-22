#ifndef __serp_click_hpp__
#define __serp_click_hpp__

class serp_click
{
private:
	std::string timestamp;
	std::string doc_id;
	int item_index;
	std::string url;
	std::string redirect;
	std::string request_id;
public:
	serp_click(std::istream& in);
	virtual ~serp_click() {}
	std::string get_doc_id() const { return doc_id; }
	std::string get_url() const { return url; }
	int get_item_index() const { return item_index; }
};

#endif // __serp_click_hpp__
