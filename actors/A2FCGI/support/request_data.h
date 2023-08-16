#ifndef _H_HTTP_A2FCGI_REQUEST_DATA_
#define _H_HTTP_A2FCGI_REQUEST_DATA_


struct REQUEST_DATA
{
	std::string request_uri;
	std::string request_method;
	std::string script_name;
	std::string content_type;
	std::string content_length;

	std::string server_software;
	std::string server_name;
	std::string gateway_interface;
	std::string server_protocol;
	std::string server_port;

	std::string remote_addr;
	std::string http_accept;
	std::string http_user_agent;
	std::string http_referer;
	std::string http_origin;

	std::string redirect_status;

	std::map<std::string,std::string> cookie;
};


#endif /* _H_HTTP_A2FCGI_REQUEST_DATA_ */