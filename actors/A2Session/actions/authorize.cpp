////////////////////////////////////////////////////////////////////////////////////////////
/**

	Выполняет авторизацию http-запроса 

*/
////////////////////////////////////////////////////////////////////////////////////////////

#define _INIT_SESSION_DEBUG_ false
#define _REQUEST_AUTHORIZATION_ false

std::string A2Session::authorize(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{
	/////////////////////////////////////////////////////////////////////////////////////
	//
	// Проверяем наличие JWT в cookie
	//
	/////////////////////////////////////////////////////////////////////////////////////


	message->data["responce"]["data"] = nlohmann::json();
	int status = 0;

	std::string cookie_token = message->http->request.cookie["tegia_token"];
	if(cookie_token != "")
	{
		status = 1;
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	// Проверяем наличие JWT в header
	//
	/////////////////////////////////////////////////////////////////////////////////////


	std::string header_token = "";
	if(message->http->request.http_authorization != "")
	{
		header_token = message->http->request.http_authorization;
		status = status + 10;
	}


	/////////////////////////////////////////////////////////////////////////////////////
	//
	// Инициализируем контекст
	//
	/////////////////////////////////////////////////////////////////////////////////////


	int jwt_status = 0;
	std::shared_ptr<tegia::threads::context> _context = std::make_shared<tegia::threads::context>();

	switch(status)
	{
		//
		// Не найдено ни одного JWT
		//
		
		case 0:
		{
			jwt_status = 404;
		}
		break;

		//
		// Найден cookie_token (пользовательский токен)
		//

		case 1:
		{
			jwt_status = _context->jwt.unpack(cookie_token, this->jwt_public_key);
		}
		break;

		//
		// Найден header_token (API токен)
		//

		case 10:
		{
			jwt_status = _context->jwt.unpack(header_token, this->jwt_public_key);
		}
		break;

		//
		// Найдены оба токена. Что делать??
		//

		case 11:
		{
			jwt_status = 400;
		}
		break;
	}


	std::cout << _OK_TEXT_ << "current status " << status << std::endl;
	std::cout << "jwt_status = " << jwt_status << std::endl;
	_context->jwt.print();


	tegia::threads::current()->context = _context;

	// std::cout << _context->jwt.json() << std::endl;

	/////////////////////////////////////////////////////////////////////////////////////
	//
	// Проверяем тип токена и ip-адрес
	//
	/////////////////////////////////////////////////////////////////////////////////////

	if(_context->jwt.get_type() == 2)
	{
		if(_context->jwt.check_ip(message->http->request.remote_addr) == false)
		{
			message->data["responce"]["type"] = "401";
			return "401";
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	// Авторизуем http-запрос
	//
	/////////////////////////////////////////////////////////////////////////////////////

	//
	// Found pattern (or 404 status)
	//

	std::string pattern = "";
	std::string actor_name = "";
	std::string action_name = "";

	auto addr = core::explode(message->http->request.script_name, "/-/");

	if(addr.size() == 2)
	{
		actor_name = addr[0].substr(8);
		action_name = "/" + addr[1];

		//
		// Добавляем метод запроса TODO: надо оптимизировать
		//

		if(message->http->request.request_method == "POST")
		{
			action_name = action_name + "/add";
		}

		if(message->http->request.request_method == "DELETE")
		{
			action_name = action_name + "/delete";
		}

		if(message->http->request.request_method == "PUT")
		{
			action_name = action_name + "/~put";
		}

		if(message->http->request.request_method == "PATCH")
		{
			action_name = action_name + "/~patch";
		}

		if(message->http->request.request_method == "CONNECT")
		{
			action_name = action_name + "/~connect";
		}

		if(message->http->request.request_method == "OPTIONS")
		{
			action_name = action_name + "/~options";
		}

		// std::cout << _YELLOW_ << "start tegia::actors::find_pattern" << _BASE_TEXT_ << std::endl;

		pattern = tegia::actors::find_pattern(actor_name,action_name);

		// std::cout << _YELLOW_ << "end tegia::actors::find_pattern" << _BASE_TEXT_ << std::endl;

		if(pattern == "")
		{
			std::cout << _ERR_TEXT_ << "not found pattern " << message->http->request.script_name << std::endl;
			message->data["responce"]["type"] = "404";
			return "404";
		}

		pattern = "/api/v3/" + actor_name + "/-" + pattern;

		std::cout << _OK_TEXT_ << "[" << actor_name << "] [" << action_name << "]" << std::endl;
		std::cout << "      " << pattern << std::endl;
	}
	else
	{
		std::cout << _ERR_TEXT_ << "not found pattern " << message->http->request.script_name << std::endl;
		message->data["responce"]["type"] = "404";
		return "404";
	}

	//
	// check authorize user (or 401 status)
	//

	#if _REQUEST_AUTHORIZATION_ == true
	if(tegia::threads::current()->get_user_id() == 0)
	{
		auto pos = this->http_public_routes.find(pattern);
		if(pos == this->http_public_routes.end())
		{
			message->data["responce"]["type"] = "401";
			return "401";
		}
		else
		{
			message->callback.push({actor_name,action_name});
			return "200";
		}
	}
	else
	{
		message->callback.push({actor_name,action_name});
		return "200";
	}
	#endif

	#if _REQUEST_AUTHORIZATION_ == false
	message->callback.push({actor_name,action_name});
	#endif

	/////////////////////////////////////////////////////////////////////////////////////
	return "200";
};
