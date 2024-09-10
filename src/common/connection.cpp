#include <tegia/core/string.h>
#include <tegia/core/cast.h>

#include "connection.h"


////////////////////////////////////////////////////////////////////////////////////////////
#undef _LOG_LEVEL_
#define _LOG_LEVEL_ _LOG_DEBUG_
#include <tegia/context/log.h>
////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////


connection_t::connection_t()
{
	this->start_time = std::chrono::high_resolution_clock::now();
	this->req = new FCGX_Request;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////


connection_t::~connection_t() 
{
	delete this->req;

	this->end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(this->end_time - this->start_time).count();

	std::cout 	<< _YELLOW_ << "[" << this->status << "] REQUEST " << _BASE_TEXT_ 
				<< this->url << " (time: " << duration << ")"  << std::endl;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////


inline std::string get_param(const char* param_name, FCGX_Request *req)
{
	if(FCGX_GetParam(param_name, req->envp) != nullptr)
	{
		return FCGX_GetParam(param_name, req->envp);
	}
	else
	{
		return "";
	}
};


bool connection_t::init(const std::string &name)
{
	//
	// REM: Записывает в лог полный список всех заголовков
	//

	std::string query;
	
	#if _LOG_LEVEL_ == _LOG_DEBUG_
	{
		int i = 0;

		query = query + "CONNECTION " + name + "\n";

		while(this->req->envp[i] != nullptr)
		{
			char * str = this->req->envp[i];
			char * position = strchr(str, '=') + sizeof(str[0]);
			int p = strlen(str) - strlen(position) - 1;
			i++;

			// std::cout << "param = " << std::string(str,p) << std::endl;
			// std::cout << "value = " << std::string(position) << std::endl;
			// std::cout << " " << std::endl;

			query = query + std::string(str,p) + " = '" + std::string(position) + "'\n";
		}

		LDEBUG(query)
	}
	#endif

	//
	// CGI Query Information
	//

	this->request_method = get_param("REQUEST_METHOD", this->req);
	this->script_name = get_param("SCRIPT_NAME", this->req);
	this->content_type = get_param("CONTENT_TYPE", this->req);
	this->content_length = get_param("CONTENT_LENGTH", this->req);
	this->redirect_status = get_param("REDIRECT_STATUS", this->req);

	//
	// Information on the server handling the HTTP/CGI request
	//
	
	this->server_software = get_param("SERVER_SOFTWARE", this->req);
	this->server_name = get_param("SERVER_NAME", this->req);
	this->gateway_interface = get_param("GATEWAY_INTERFACE", this->req);
	this->server_protocol = get_param("SERVER_PROTOCOL", this->req);
	this->server_port = get_param("SERVER_PORT", this->req);
	
	//
	// Remote User Information. Information about the user making the CGI request
	//

	this->remote_addr = get_param("REMOTE_ADDR", this->req);
	this->http_accept = get_param("HTTP_ACCEPT", this->req);
	this->http_user_agent = get_param("HTTP_USER_AGENT", this->req);
	this->http_referer = get_param("HTTP_REFERER", this->req);
	this->http_origin = get_param("HTTP_ORIGIN", this->req);
	this->http_authorization = get_param("HTTP_AUTHORIZATION", this->req);

	//
	// Get a COOKIE
	//

	if(FCGX_GetParam("HTTP_COOKIE", this->req->envp) != nullptr)
	{
		std::string cookie = std::string(FCGX_GetParam("HTTP_COOKIE", this->req->envp));

		#if _LOG_LEVEL_ == _LOG_DEBUG_
		{
			query = query + "Cookies: " + cookie + "\n";
			LDEBUG(query);
		}
		#endif

		auto cookie_arr = tegia::string::explode(cookie,"; ");
		for(auto it = cookie_arr.begin(); it != cookie_arr.end(); it++)
		{
			auto tmp = tegia::string::explode((*it),"=");
			this->cookie.insert({tmp[0],tmp[1]});
		}
	}


	//
	// Get a POST data
	//

	int content_length = 0;
	if(this->content_length != "")
	{
		content_length = core::cast<int>(this->content_length);
	}
	
	if(content_length > 0)
	{
		char *buff = new char[content_length+1]{};
		FCGX_GetStr(buff,content_length+1,this->req->in);
		std::string post = std::string(buff, content_length);
		delete buff;

		#if _LOG_LEVEL_ == _LOG_DEBUG_
		{
			query = query + "POST: " + post + "\n";
			LDEBUG(query);
		}
		#endif

		//
		// Проверяем, что POST-данные содержат валидный JSON
		//

		try
		{
			nlohmann::json _post = nlohmann::json::parse(post);
			if(_post.is_array() == true)
			{
				this->post["post"] = _post;
			}
			else
			{
				this->post = _post;
			}
		}

		//
		// Обрабатываем классические POST-данные
		//

		catch(nlohmann::json::parse_error& e)
		{
			LDEBUG("POST RAW data: " + post);
			std::cout << "POST RAW data: " << post << std::endl;

			/*
			auto params = core::explode(post,"&",true);
			for(auto it = params.begin(); it != params.end(); it++)
			{
				auto param = core::explode( (*it),"=",true);
				if(param.size() == 1)
				{
					message->http->request.post[param[0]] = ""; 
				}
				else
				{
					message->http->request.post[param[0]] = tegia::http::unescape(param[1]); 
				}
			}
			*/
		}	
	}



	return true;
};



