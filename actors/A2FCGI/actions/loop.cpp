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

	std::thread::id this_id = std::this_thread::get_id();
	std::cout << _OK_TEXT_ << "fcgi listener thread " << this_id << " run...\n";
	
	//
	// Инициализируем порт   
	//

	std::string host = message->data["host"].get<std::string>();
	if(this->init_port(host, "FastCGI") == false)
	{
		std::cout << _ERR_TEXT_ << "port [" << host << "] init error" << std::endl;
		exit(1); 
	}

	//
	// Берем порт на "прослушку"
	//

	int rc = 0;
	std::mutex accept_mutex;
	bool work = true;

	while(work)
	{
		// TODO: Реализовать завершение цикла если необходимо остановить Платформу 
		try
		{
			std::string uuid = tegia::random::uuid();
			std::string actor_name = "session/" + uuid;
			nlohmann::json data = nlohmann::json::object();
			A2Session *session = reinterpret_cast<A2Session *>(tegia::actors::_new("A2Session", actor_name, data, true));

			session->port = this->port;
			session->maxage = this->maxage;
			session->alloworigin = this->alloworigin;
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
		}

		catch(const std::exception&) 
		{
			// TODO: handle error condition
		}  
	}  

	/////////////////////////////////////////////////////////////////////////////////////////  
	return _STATUS_;
};

