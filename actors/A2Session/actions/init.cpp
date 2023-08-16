////////////////////////////////////////////////////////////////////////////////////////////
/**

	Инициализация http-сессии

*/
////////////////////////////////////////////////////////////////////////////////////////////


inline std::string get_param(const char* param_name, CRequest *request)
{
	if(FCGX_GetParam(param_name, request->req->envp) != nullptr)
	{
		return FCGX_GetParam(param_name, request->req->envp);
	}
	else
	{
		return "";
	}
};


std::string A2Session::init(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{
	int _STATUS_ = 0;
	/////////////////////////////////////////////////////////////////////////////////////////  


	message->http = new HTTP();

	/*

	REM: Раскомментировать этот код, если нужно получить полный список всех заголовков

	int i = 0;
	while(request->req->envp[i] != nullptr)
	{
		char * str = request->req->envp[i];
		char * position = strchr(str, '=') + sizeof(str[0]);
		int p = strlen(str) - strlen(position) - 1;
		i++;

		std::cout << "param = " << std::string(str,p) << std::endl;
		std::cout << "value = " << std::string(position) << std::endl;
		std::cout << " " << std::endl;
	}
	
	*/

	// --------------------------------------------------------------------------------------
	//
	// CGI Query Information
	//
	// --------------------------------------------------------------------------------------


	message->http->request.request_method = get_param("REQUEST_METHOD", request);
	message->http->request.script_name = get_param("SCRIPT_NAME", request);
	message->http->request.content_type = get_param("CONTENT_TYPE", request);
	message->http->request.content_length = get_param("CONTENT_LENGTH", request);
	message->http->request.redirect_status = get_param("REDIRECT_STATUS", request);

	request->url = message->http->request.script_name;


	// --------------------------------------------------------------------------------------
	//
	// Information on the server handling the HTTP/CGI request
	//
	// --------------------------------------------------------------------------------------

	
	message->http->request.server_software = get_param("SERVER_SOFTWARE", request);
	message->http->request.server_name = get_param("SERVER_NAME", request);
	message->http->request.gateway_interface = get_param("GATEWAY_INTERFACE", request);
	message->http->request.server_protocol = get_param("SERVER_PROTOCOL", request);
	message->http->request.server_port = get_param("SERVER_PORT", request);
	

	// --------------------------------------------------------------------------------------
	//
	// Remote User Information. Information about the user making the CGI request
	//
	// --------------------------------------------------------------------------------------

	
	message->http->request.remote_addr = get_param("REMOTE_ADDR", request);
	message->http->request.http_accept = get_param("HTTP_ACCEPT", request);
	message->http->request.http_user_agent = get_param("HTTP_USER_AGENT", request);
	message->http->request.http_referer = get_param("HTTP_REFERER", request);
	message->http->request.http_origin = get_param("HTTP_ORIGIN", request);
	message->http->request.http_authorization = get_param("HTTP_AUTHORIZATION", request);
	

	// --------------------------------------------------------------------------------------
	//
	// Get a COOKIE
	//
	// --------------------------------------------------------------------------------------

	if(FCGX_GetParam("HTTP_COOKIE", request->req->envp) != nullptr)
	{
		std::string cookie = std::string(FCGX_GetParam("HTTP_COOKIE", request->req->envp));
		auto cookie_arr = core::explode(cookie,"; ");
		for(auto it = cookie_arr.begin(); it != cookie_arr.end(); it++)
		{
			auto tmp = core::explode((*it),"=");
			message->http->request.cookie.insert({tmp[0],tmp[1]});
		}
	}
	
	// -------------------------------------------------------------------------
	//
	// Get a POST data
	//
	// -------------------------------------------------------------------------

	int content_length = 0;
	if(message->http->request.content_length != "")
	{
		content_length = core::cast<int>(message->http->request.content_length);
	}
	
	if(content_length > 0)
	{
		char *buff = new char[content_length+1]{};
		FCGX_GetStr(buff,content_length+1,request->req->in);
		std::string post = std::string(buff, content_length);
		delete buff;

		//
		// Проверяем, что POST-данные содержат валидный JSON
		//

		try
		{
			nlohmann::json _post = nlohmann::json::parse(post);
			if(_post.is_array() == true)
			{
				message->http->request.post["post"] = _post;
			}
			else
			{
				message->http->request.post = _post;
			}
		}

		//
		// Обрабатываем классические POST-данные
		//

		catch(nlohmann::json::parse_error& e)
		{
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
					message->http->request.post[param[0]] = core::url::decode(param[1]); 
				}
			}
		}	

		message->data["request"] = message->http->request.post;
	}

	// -------------------------------------------------------------------------
	//
	// Get a QueryString
	//
	// -------------------------------------------------------------------------

	std::string query_param = get_param("QUERY_STRING", request);

	if(query_param.length() > 0)
	{
		auto v_query_params = core::explode(query_param, "&", false);
		for (auto it = v_query_params.begin(); it != v_query_params.end(); it++)
		{
			auto v_query_param = core::explode( (*it), "=", false);
			if(v_query_param.size() == 2)
			{
				CURL *curl = curl_easy_init();

				std::string value = v_query_param[1];
				char * decoded = curl_easy_unescape(curl, value.c_str(), 0, nullptr);
				value = decoded;
				curl_free(decoded);
				message->http->request.query_param[v_query_param[0]] = value;
				message->data["request"][v_query_param[0]] = value;

				curl_easy_cleanup( curl );
			}
			else
			{
				if (it->find("=") != std::string::npos)
				{
					message->http->request.query_param[v_query_param[0]] = "";
					message->data["request"][v_query_param[0]] = "";
				}
				else
				{
					message->http->request.query_param[v_query_param[0]] = "true";
					message->data["request"][v_query_param[0]] = "true";
				}
			}
		}
	}
	
	message->data["responce"]["status"] = "Status: 200 OK\r\n";
	message->data["responce"]["type"]   = "json";	// Возвращает JSON ответ
													// = "file"   // Отдает файл на скачивание
	message->data["responce"]["cache"]  = "Cache-Control: no-cache\r\n";
	message->data["responce"]["header"] = "Content-Type: application/json; charset=utf-8\r\n";
	message->data["responce"]["data"] = nlohmann::json();

	#if _HTTP_PARCE_TEST_ == true
	// std::cout << message->data.dump() << std::endl;
	std::cout << message->http->request.json().dump() << std::endl;
	#endif	

	_STATUS_ = 200;
	
	/////////////////////////////////////////////////////////////////////////////////////  
	return core::cast<std::string>(_STATUS_);
};
