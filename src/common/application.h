#ifndef H_HTTP_APPLICATION
#define H_HTTP_APPLICATION

#include <string>
#include <unordered_map>

#include <iostream>

#include <tegia/core/json.h>

class application_t
{
	public:
		application_t(){};
		~application_t() = default;

		std::string name;
		
		//
		// cookie
		//
		
		bool cookie_http_only;
		long long int cookie_maxage = 0;
		std::string cookie_same_site;
		std::vector<std::string> domains;

		//
		// CORS
		//

		bool cors_alloworigin;

		//
		// APPLICATION ACTOR
		//

		std::string actor;
		std::string action;
};



class applications_t
{
	public:
		applications_t() = default;
		~applications_t() = default;


		std::tuple<bool, application_t * const> get(const std::string &domain)
		{
			std::cout << "apps->get() " << domain << std::endl;

			auto pos = this->apps.find(domain);
			if(pos == this->apps.end())
			{
				std::cout << "not found domain " << domain << std::endl;
				
				return std::move(std::make_tuple(false,nullptr));
			}

			return std::move(std::make_tuple(true,pos->second));
		};


		bool add(const nlohmann::json &config)
		{
			//
			// TODO проверить, что приложений с таким доменом пока не существует
			//

			auto app = new application_t();
			app->name = config["name"].get<std::string>();
			app->cookie_http_only = config["cookie"]["http_only"].get<bool>();
			app->cookie_maxage    = config["cookie"]["maxage"].get<long long int>();
			app->cookie_same_site = config["cookie"]["same_site"].get<std::string>();
			app->cors_alloworigin = config["cors"]["alloworigin"].get<bool>();

			app->actor = config["router"]["actor"].get<std::string>();
			app->action = config["router"]["action"].get<std::string>();

			//
			// DOMAINS
			//

			for(auto domain = config["domains"].begin(); domain != config["domains"].end(); ++domain)
			{
				std::string domain_name = domain->get<std::string>();
				const auto [_app, success] = apps.insert({domain_name,app});

				if(success)
				{
					app->domains.push_back(domain_name);
					// std::cout << _OK_TEXT_ << "domain '" << domain_name << "' success added" << std::endl;
					// std::cout << "app name '" << _app->second->name << std::endl;
				}
				else
				{
					// std::cout << _ERR_TEXT_ << "domain '" << domain_name << "' not added" << std::endl;
					// std::cout << "app name '" << _app->second->name << std::endl;
				}
			}

			return true;
		};

	private:
		std::unordered_map<
			std::string,		// application domain
			application_t *		// application data object
		> apps;
};


#endif