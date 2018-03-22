#include <exception>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

#include "serp_page.hpp"
#include "serp_click.hpp"
#include "unattached_clicks.hpp"
#include "bcdata_url.hpp"

int main(int argc, char* argv[]) {

	std::ios::sync_with_stdio(false);
	std::cin.tie(NULL);
	int ret = 0;

	std::string date = "default";
	enum class gserp_actions { Main, DumpClicks };
	gserp_actions action =  gserp_actions::Main;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i],"-c")==0)
			action = gserp_actions::DumpClicks;
		if (strcmp(argv[i],"-d")==0 && i + 1 <argc)
			date = argv[i+1];
	}

	if (action == gserp_actions::DumpClicks)
		return unattached_clicks::dump_clicks (date, std::cin);

	std::ofstream gs_backlinks;
	std::ofstream deb;
	std::ofstream gs_clicks;
	std::ofstream stspos;
	std::ofstream google_site_clicks;
	std::ofstream show;
	std::ofstream image_queries;
	std::ofstream rawq;
	std::ofstream facebook_snippets;
	std::ofstream domain_url;
	std::ofstream date_domain_google;

	gs_backlinks.open (".//output//gs_backlinks-"+date);
	deb.open (".//output//deb-"+date);
	gs_clicks.open (".//output//clicks-"+date);
	stspos.open (".//output//stspos-"+date);
	google_site_clicks.open (".//output//google_site_clicks-"+date);
	show.open (".//output//show-"+date);
	image_queries.open (".//output//image_queries-"+date);
	rawq.open (".//output//rawq-"+date);
	facebook_snippets.open (".//output//facebook_snippets-"+date);
	domain_url.open(".//output//domain_url-"+date);
	date_domain_google.open(".//output//date_domain_google-"+date);

	try {
		std::string line;
		unattached_clicks clicks (date);
		std::string user_id;
		std::set <std::string> queries;
		while (std::getline(std::cin, line)) {
			try {
				if (line.find ("==>\t") == 0) {
					user_id = line.substr (strlen("==>\t"));
					for (std::string query : queries)
						rawq << query << "\n";
					queries.clear();
				}
				if (line == "<page_begin>") {
					serp_page sp (std::cin);
					if (sp.get_query() == "")
						continue;
					int organic_pos = sp.get_search_start();
					int noads_pos = sp.get_search_start();
					queries.insert (sp.get_query());
					for (serp_item item : sp.get_results()) {
						bcdata_normalized_url bcurl (item.get_url());
						if (item.has_date_snippet() && item.can_get_date()){
							date_domain_google << bcurl.get_url() << '\n';
							date_domain_google << item.get_date() << '\n';

							domain_url << item.get_url() << '\n';
						}
						// } else {
						// 	date_domain_google << bcurl.get_host() << " 0\n";
						// }
						if (clicks.find (std::make_pair (sp.get_doc_id(), item.get_index())) != clicks.end())  {
							std::string url = clicks[std::make_pair (sp.get_doc_id(), item.get_index())];
							if (item.get_type() == "organic") {
								gs_backlinks << url << ' ' << sp.get_query() <<  "\n";
								deb << url << ' ' << sp.get_query() << "\t="  << item.get_title() << "\t=" << item.get_description() << "\n";
								gs_clicks << sp.get_query() << "\t" << url << "\t" << organic_pos << "\n";
								stspos << url << ' ' << sp.get_query() << ' ' << user_id << ' ' << organic_pos << "\n";
								google_site_clicks << sp.get_timestamp() << "\t" << user_id << "\t2\t" << sp.get_query() << "\t" << url <<  "\n";
							} else if (item.get_type() == "top-ad" || item.get_type() == "bottom-ad") {
								google_site_clicks << sp.get_timestamp() << "\t" << user_id << "\t1\t" << sp.get_query() << "\t" <<  item.get_url() <<  "\n";
							}
						}
						if (item.get_type() == "organic") {
							show << sp.get_query() << "\t" << item.get_url() << "\t" << organic_pos << "\n";
							if (bcurl.get_level2_domain() == "facebook.com" && (bcurl.get_depth() == 1 || (bcurl.path_start_from ("/pages/") && bcurl.get_depth() == 2)))
								facebook_snippets << item.get_url() << "\t" << sp.get_query() << "\t"  << item.get_title() << "\t" << item.get_description() << "\n";
							organic_pos++;
						} else if (item.get_type() == "image") {
							std::string query = sp.get_query();
							std::replace(query.begin(), query.end(), '\t', ' ');
							image_queries << query << "\t" << noads_pos << "\n";
						}
						if (item.get_type() != "top-ad" && item.get_type() != "bottom-ad")
							noads_pos++;
					}
				}
			}
			catch  (const std::exception& e)  {
				std::cout << e.what() << " at " << line << "\n";
			}
		}
		for (std::string query : queries)
			rawq << query << "\n";
	}
	catch (const std::exception& e)  {
		std::cout << "main: "  << e.what() << "\n";
		ret = -1;
	}

	gs_backlinks.close();
	deb.close();
	gs_clicks.close();
	stspos.close();
	google_site_clicks.close();
	show.close();
	image_queries.close();
	rawq.close();
	facebook_snippets.close();
	date_domain_google.close();

	return ret;
}
