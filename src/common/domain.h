#ifndef H_HTTP_DOMAIN
#define H_HTTP_DOMAIN

#include <string>
#include <unordered_map>

#include <iostream>

#include <tegia/core/json.h>

#include "connection.h"

class domain_t
{
	public:
		domain_t();
		~domain_t() = default;

		bool auth(connection_t * connection, const std::string &wsid);
		std::string jwt_public_key();

		std::string name;
		
		//
		// cookie
		//
		
		bool cookie_http_only;
		long long int cookie_maxage = 0;
		std::string cookie_same_site;
		std::string domain;

		//
		// CORS
		//

		bool cors_alloworigin;

		//
		// APPLICATION ACTOR
		//

		std::string actor;
		std::string action;

		std::string pub_key_path{};
		std::string priv_key_path{};
};



class domains_t
{
	public:
		domains_t() = default;
		~domains_t() = default;


		std::tuple<bool, domain_t * const> get(const std::string &domain)
		{
			auto pos = this->domains.find(domain);
			if(pos == this->domains.end())
			{
				std::cout << "not found domain " << domain << std::endl;
				
				return std::move(std::make_tuple(false,nullptr));
			}

			return std::move(std::make_tuple(true,pos->second));
		};


		bool add(const nlohmann::json &config)
		{
			//
			// TODO проверить, что таких доменов пока не существует
			//

			auto domain = new domain_t();
			domain->name = config["name"].get<std::string>();
			domain->cookie_http_only = config["cookie"]["http_only"].get<bool>();
			domain->cookie_maxage    = config["cookie"]["maxage"].get<long long int>();
			domain->cookie_same_site = config["cookie"]["same_site"].get<std::string>();
			domain->cors_alloworigin = config["cors"]["alloworigin"].get<bool>();

			const auto [_domain, success] = domains.insert({domain->name,domain});

			if(success)
			{
				std::cout << _OK_TEXT_ << "domain '" << domain->name << "' success added" << std::endl;
			}
			else
			{
				std::cout << _ERR_TEXT_ << "domain '" << domain->name << "' not added" << std::endl;
			}

			return true;
		};

	private:
		std::unordered_map<
			std::string,		// domain name
			domain_t *		    // domain data object
		> domains;
};


#endif