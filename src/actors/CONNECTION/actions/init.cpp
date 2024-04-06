////////////////////////////////////////////////////////////////////////////////////////////
/**

	\brief Обработчик сообщения /init
	\detail

	Сценарий обработки connection:
	1. /init - парсим параметры запроса
	2. /auth - авторизуем пользователя
	3. /route - по доменному имени определяем какое запрашивается приложение
	4. Отправляем запрос в соответствующее приложение, указывая callback для отправки ответа
	5. /responce - отправляем ответ
	6. "выгружаем" актор соединения
		
*/   
////////////////////////////////////////////////////////////////////////////////////////////

namespace HTTP {


inline std::string get_param(const char* param_name, Connection_t *connection)
{
	if(FCGX_GetParam(param_name, connection->req->envp) != nullptr)
	{
		return FCGX_GetParam(param_name, connection->req->envp);
	}
	else
	{
		return "";
	}
};



int CONNECTION::init(const std::shared_ptr<message_t> &message)
{
	int _STATUS_ = 200;
	///////////////////////////////////////////////////////////////////////////////////////// 

	auto msg = std::dynamic_pointer_cast<message_http_t>(message);
	if(!msg)
	{
		return 400;
	}
	
	this->connection = msg->_connection;


	/*
	// REM: Раскомментировать этот код, если нужно получить полный список всех заголовков

	int i = 0;
	while(this->connection->req->envp[i] != nullptr)
	{
		char * str = this->connection->req->envp[i];
		char * position = strchr(str, '=') + sizeof(str[0]);
		int p = strlen(str) - strlen(position) - 1;
		i++;

		std::cout << "param = " << std::string(str,p) << std::endl;
		std::cout << "value = " << std::string(position) << std::endl;
		std::cout << " " << std::endl;
	}
	*/

	//
	// CGI Query Information
	//

	this->connection->request_method = get_param("REQUEST_METHOD", this->connection);
	this->connection->script_name = get_param("SCRIPT_NAME", this->connection);
	this->connection->content_type = get_param("CONTENT_TYPE", this->connection);
	this->connection->content_length = get_param("CONTENT_LENGTH", this->connection);
	this->connection->redirect_status = get_param("REDIRECT_STATUS", this->connection);

	//
	// Information on the server handling the HTTP/CGI request
	//
	
	this->connection->server_software = get_param("SERVER_SOFTWARE", this->connection);
	this->connection->server_name = get_param("SERVER_NAME", this->connection);
	this->connection->gateway_interface = get_param("GATEWAY_INTERFACE", this->connection);
	this->connection->server_protocol = get_param("SERVER_PROTOCOL", this->connection);
	this->connection->server_port = get_param("SERVER_PORT", this->connection);
	
	//
	// Remote User Information. Information about the user making the CGI request
	//

	this->connection->remote_addr = get_param("REMOTE_ADDR", this->connection);
	this->connection->http_accept = get_param("HTTP_ACCEPT", this->connection);
	this->connection->http_user_agent = get_param("HTTP_USER_AGENT", this->connection);
	this->connection->http_referer = get_param("HTTP_REFERER", this->connection);
	this->connection->http_origin = get_param("HTTP_ORIGIN", this->connection);
	this->connection->http_authorization = get_param("HTTP_AUTHORIZATION", this->connection);

	//
	// Get a COOKIE
	//

	if(FCGX_GetParam("HTTP_COOKIE", this->connection->req->envp) != nullptr)
	{
		std::string cookie = std::string(FCGX_GetParam("HTTP_COOKIE", this->connection->req->envp));
		auto cookie_arr = tegia::string::explode(cookie,"; ");
		for(auto it = cookie_arr.begin(); it != cookie_arr.end(); it++)
		{
			auto tmp = tegia::string::explode((*it),"=");
			this->connection->cookie.insert({tmp[0],tmp[1]});
		}
	}

	//
	// Get a POST data
	//


	/*
		curl -X POST http://app.my-check.local/api/v3/contacts/search/-/search -H 'Content-Type: application/json' -d '{"type":"email","contact":"ivandos34rus@mail.ru"}'
	*/

	int content_length = 0;
	if(this->connection->content_length != "")
	{
		content_length = core::cast<int>(this->connection->content_length);
	}
	
	if(content_length > 0)
	{
		char *buff = new char[content_length+1]{};
		FCGX_GetStr(buff,content_length+1,this->connection->req->in);
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
				this->connection->post["post"] = _post;
			}
			else
			{
				this->connection->post = _post;
			}
		}

		//
		// Обрабатываем классические POST-данные
		//

		catch(nlohmann::json::parse_error& e)
		{
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

		// std::cout << "POST data: " << this->connection->post << std::endl;

		// message->data["http"]["request"] = this->connection->post;
	}

	//
	// TEST AUTH
	//



	//
	// FOUND APPLICATION
	//

	bool res;
	std::tie(res,this->app) = msg->apps->get(this->connection->server_name);
	if(res == false)
	{
		std::cout << _ERR_TEXT_ << "not found app name for domain '" << this->connection->server_name << "'" << std::endl;

		auto _msg = std::make_shared<message_t>();
		_msg->http["response"]["status"] = 434;	// Запрашиваемый хост недоступен
		_msg->http["response"]["type"] = "application/json";
		tegia::message::send(this->name,"/response",_msg);
		return 434;
	}

	//
	// Аутентифицируемся
	//

	this->app->auth(this->connection);
	
	//
	// Отправляем запрос на приложение
	//

	auto _msg = std::make_shared<message_t>();
	_msg->http["request"] = this->connection->json();
	_msg->http["application"]["name"] = app->name;
	_msg->callback.add(this->name,"/response");


	tegia::message::send(
		this->app->actor,
		this->app->action,
		_msg);


	/////////////////////////////////////////////////////////////////////////////////////////  
	return _STATUS_;
};

}	// END namespace HTTP
