#ifndef H_HTTP_REQUEST_DATA
#define H_HTTP_REQUEST_DATA

#include <string>
#include <map>

#include <tegia/core/json.h>


class HTTP_REQUEST_DATA
{
	public:

		std::string request_method{};
		std::string script_name{};
		std::string content_type{};
		std::string content_length{};
		std::string redirect_status{};

		std::string server_software{};
		std::string server_name{};
		std::string gateway_interface{};
		std::string server_protocol{};
		std::string server_port{};

		std::string remote_addr{};
		std::string http_accept{};
		std::string http_user_agent{};
		std::string http_referer{};
		std::string http_origin{};
		std::string http_authorization{};

		nlohmann::json post = nlohmann::json();
		nlohmann::json query_param = nlohmann::json();

		std::string get_cookie(const std::string &name)
		{
			return {};
		};

	private:

		std::map<std::string,std::string> cookie{};
};

#endif