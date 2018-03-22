#include <string>
#include <cstring>
#include <algorithm>

#include "bcdata_url.hpp"

std::set <std::string>  bcdata_url::level_3_host_domains = {"ac.vn", "biz.vn", "com.vn", "edu.vn", "gov.vn", "health.vn", "info.vn", "int.vn", "name.vn", "net.vn", "org.vn", "pro.vn"};

void bcdata_url::normalize() {
	size_t pos = 0;
	if (url.compare (0, strlen ("http://"), "http://") == 0)
		pos = strlen ("http://");
	else if (url.compare (0, strlen ("https://"), "https://") == 0)
		pos = strlen ("https://");
	if (url.compare (pos, strlen ("www."), "www.") == 0)
		pos += strlen ("www.");
	else if (url.compare (pos, strlen ("hn."), "hn.") == 0)
		pos += strlen ("hn.");
	else if (url.compare (pos, strlen ("hcm."), "hcm.") == 0)
		pos += strlen ("hcm.");
	if (url.find (".", pos) != std::string::npos)	
		url.erase (0, pos);
}

void bcdata_url::end_slash() {
	size_t pos = url.find ("://");
	pos = pos == std::string::npos ? url.find_first_of("/?#") : url.find_first_of("/?#", pos + 3);
	if (pos == std::string::npos)
		url.push_back('/');
	else if (url.at (pos) != '/')
		url.insert (pos, "/");
}

bool bcdata_url::validate() const {
	if (url.length() > max_url_length)
		return false;
	std::string host = get_host();
	if (host.find_first_of ('.') == std::string::npos)
		return false;
	if (host.find_first_not_of ("abcdefghijklmnopqrstuvwxyz0123456789-.") != std::string::npos)
		return false;
	if (host.front() == '.' || host.front() == '-' || host.back() == '.' || host.back() == '-')
		return false;
	if (host.find ("..") != std::string::npos || host.find ("-.") != std::string::npos || host.find (".-") != std::string::npos)
		return false;
	return true;
} 

std::string bcdata_url::get_site (bool second_level) const {
	std::string site = get_host();
	if (site.compare (0, strlen ("www."), "www.") == 0)
		site.erase (0, strlen ("www."));
	else 
		if (site.compare (0, strlen("www"), "www") == 0 && site[3] >= '0' && site[3] <= '9' && site[4] == '.')
			site.erase (0, strlen("www.") + 1);
	size_t dotpos = std::string::npos;
	int ndots = 0;
	int dotslimit = second_level ? 1 : 2;
	while ((dotpos = site.rfind ('.', dotpos)) != std::string::npos) {
		ndots++;
		if (ndots == 2 && level_3_host_domains.find (site.substr (dotpos + 1, site.length() - dotpos - 1)) != level_3_host_domains.end())
			dotslimit++;
		if (ndots > dotslimit) {
			site.erase (0, dotpos + 1);
			break;
		}
		dotpos --;
	 }
	 return site;
}

std::string bcdata_url::get_host() const {
	size_t first = 0;
    if (url.compare (0, strlen ("http://"), "http://") == 0)
		first = strlen ("http://");
	else if (url.compare (0, strlen ("https://"), "https://") == 0)
		first = strlen ("https://");
	size_t last = url.find_first_of ("/:", first);
	return url.substr (first, last == std::string::npos ? std::string::npos : last - first); 
}

std::string bcdata_url::get_level2_domain() const {
	std::string host = get_host();
	size_t pos = host.rfind ('.');
	if (pos != std::string::npos)
		pos = host.rfind ('.', pos - 1);
	return pos == std::string::npos ? host : host.substr (pos + 1);
}

std::string bcdata_url::get_path() const {
	size_t pos = 0;
    if (url.compare (0, strlen ("http://"), "http://") == 0)
		pos = strlen ("http://");
	else if (url.compare (0, strlen ("https://"), "https://") == 0)
		pos = strlen ("https://");
	size_t first = url.find('/', pos);
	size_t last = url.find_first_of("?#", first);
	return url.substr (first, last == std::string::npos ? std::string::npos : last - first);
}

std::string bcdata_url::get_query_string() const {
	size_t pos = url.find_first_of ("?#");
	return pos == std::string::npos ? "" : url.substr (pos + 1);
}


std::string bcdata_url::get_param (const char *name,  bcdata_url_decode_options decode) const {
	std::string qs = "&"; 
	qs.append (get_query_string());
	std::string pat = "&"; 
	pat.append (name); 
	pat.append ("=");
	size_t first = qs.find (pat);
	if (first == std::string::npos)
		return "";
	first += pat.length();	
	size_t last = qs.find ("&", first);
	std::string value = last == std::string::npos ? qs.substr(first) : qs.substr(first, last - first);
	if (decode != bcdata_url_decode_options::No) {
		std::string decoded;
		bool changed; 
		do { 
			size_t pos = 0;
			changed = false;
			std::string decoded;
			for (size_t i = 0; i < value.length(); i++) {
				if (value[i] == '%' && i + 2 < value.length() && isxdigit(value[i+1]) &&  isxdigit(value[i+2]))  {
					int ic;
					sscanf (value.substr(i+1,2).c_str(), "%x", &ic);
					char c = static_cast<char>(ic);
					decoded += c;
					i += 2;
					changed = true;
				} else if (value[i] == '+') {
					decoded += ' ';
					changed = true;
				} else
					decoded += value[i];
			}
			value = decoded;
		} while (changed && decode == bcdata_url_decode_options::Recursive);
	}
	return value;
}

bool  bcdata_url::has_param (const char *name) const {
	std::string qs = "&";
	qs.append (get_query_string());
	std::string pat = "&";
	pat.append (name);
	pat.append ("=");
	return qs.find (pat) == std::string::npos ? false : true;
}

bool bcdata_url::path_start_from (const char* str) const {
	return !get_path().compare(0, strlen(str), str);
}

size_t bcdata_url::get_depth() const {
	std::string path = get_path();
	size_t depth = std::count (path.begin(), path.end(), '/');
	if (path.back() == '/')
		depth--;
	if (get_query_string() != "") 
		depth++;
	return depth;	
}

bool bcdata_raw_url::validate() const {
	if (get_url().compare (0, strlen ("http://"), "http://") != 0 && get_url().compare (0, strlen ("https://"), "https://") != 0)
		return false;
	return bcdata_url::validate();
}

