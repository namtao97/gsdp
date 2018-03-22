#ifndef __unattached_clicks_hpp__
#define __unattached_clicks_hpp__

#include <map>

class unattached_clicks : public std::map <std::pair <std::string, int>, std::string>  {
private:
public:
	unattached_clicks (const std::string date);
	virtual ~unattached_clicks() {}
	static int dump_clicks (const std::string date, std::istream &in);
};

#endif // __unattached_clicks_hpp__
