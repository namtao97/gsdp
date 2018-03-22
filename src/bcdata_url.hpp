#ifndef __bcdata_url_hpp__
#define __bcdata_url_hpp__

#include <string>
#include <set>

enum class bcdata_url_decode_options { No, Once, Recursive};

class bcdata_url {
private:
    std::string url;
    static std::set <std::string> level_3_host_domains;
	static const size_t max_url_length = 2083;
protected: 
	void normalize();
    void end_slash();
public:
    virtual ~bcdata_url() {}
    bcdata_url(const std::string &u) : url(u) {} 
    bool operator== (const bcdata_url& other) const { return this->get_url() == other.get_url(); } 
    bool operator< (const bcdata_url& other) const { return this->get_url() < other.get_url(); }
    virtual bool validate() const;
	const std::string& get_url() const { return url; }  
    std::string get_site (bool second_level = false) const;
    std::string get_host() const;
	std::string get_level2_domain() const;	
	std::string get_path() const;
	std::string get_query_string() const;
	std::string get_param (const char *name,  bcdata_url_decode_options decode = bcdata_url_decode_options::No) const;
    bool has_param (const char *name) const;
	bool path_start_from (const char* str) const;
    size_t get_depth() const;
};



class bcdata_normalized_url : public bcdata_url {
public:
    bcdata_normalized_url(const std::string &u, bool add_end_slash = true) : bcdata_url{u} { 
		if (add_end_slash) 
			end_slash(); 
		normalize(); 
	}
    virtual ~bcdata_normalized_url() {}
};

class bcdata_raw_url : public bcdata_url {
public:
    bcdata_raw_url(const std::string &u) : bcdata_url{u} {
		end_slash();
	}
    virtual ~bcdata_raw_url() {}
    virtual bool validate() const;
};

#endif // __bcdata_url_hpp__

