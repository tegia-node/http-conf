////////////////////////////////////////////////////////////////////////////////////////////
/**

	\brief Обработчик сообщения /loop
	\detail
		Сообщение init инициализирует порт, указанный в теле сообщения и запускает цикл 
		приема fcgi-соединений. В теле сообщения должна быть строка вида: "127.0.0.1:9022"

*/   
////////////////////////////////////////////////////////////////////////////////////////////

std::string A2FCGI::loop(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{
	std::string _STATUS_ = "200";
	/////////////////////////////////////////////////////////////////////////////////////////  

	try
	{
		//
		// Инициализируем параметры
		//

		this->validator.validate(message->data);
		_params * params = new _params();

		// fcgi
		params->fcgi.host = message->data["fcgi"]["host"].get<std::string>();
		params->fcgi.listen_queue_backlog = message->data["fcgi"]["listen_queue_backlog"].get<int>();

		// cookie
		params->cookie.maxage = message->data["cookie"]["maxage"].get<long long int>();
		params->cookie.same_site = message->data["cookie"]["same_site"].get<std::string>();
		params->cookie.http_only = message->data["cookie"]["http_only"].get<bool>();

		// cors
		params->cors.alloworigin = message->data["cors"]["alloworigin"].get<bool>();

		//
		// Инициализируем порт   
		//

		if(this->init_port(params) == false)
		{
			std::cout << _ERR_TEXT_ << "port [" << params->fcgi.host << "] init error" << std::endl;
			exit(1); 
		}

		//
		// Слушаем порт
		//

		int rc = 0;
		std::mutex accept_mutex;
		bool work = true;		

		while(work)
		{
			// TODO: Реализовать завершение цикла если необходимо остановить Платформу 
	
			std::string uuid = tegia::random::uuid();
			std::string actor_name = "session/" + uuid;
			nlohmann::json data = nlohmann::json::object();
			A2Session *session = reinterpret_cast<A2Session *>(tegia::actors::_new("A2Session", actor_name, data, true));

			session->params = params;
			session->uuid = uuid;

			FCGX_Init();
			FCGX_InitRequest(session->request->req,  this->listen_socket, 0);
			rc = FCGX_Accept_r(session->request->req);

			if (rc < 0)
			{
				//
				// TODO: обработать ошибку
				//

				std::cout << _ERR_TEXT_ << "ERROR REQUEST" << std::endl;
			}
			else
			{
				auto msg = tegia::message::init();
				msg->callback.push({this->name, "/end/" + uuid});
				msg->callback.push({actor_name, "/send"});
				msg->callback.push({"sessions","/authorize"});
				tegia::message::send(actor_name, "/init", msg, _PHIGHT_);
			}  
		}	// END while(work)
	}

	//
	// Обработка исключений
	//

	catch(const std::exception& e)
	{
		std::cout << _ERR_TEXT_ << e.what() << '\n';
		exit(0);
	}
	
	



	/////////////////////////////////////////////////////////////////////////////////////////  
	return _STATUS_;
};

