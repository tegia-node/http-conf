#undef _LOG_LEVEL_
#define _LOG_LEVEL_ _LOG_NOTICE_
#include <tegia2/context/log.h>

////////////////////////////////////////////////////////////////////////////////////////////
/**

	\brief Обработчик сообщения /init
	\detail
		Сообщение init инициализирует порт, указанный в теле сообщения и запускает цикл 
		приема fcgi-соединений.

*/   
////////////////////////////////////////////////////////////////////////////////////////////

namespace HTTP {

int LISTENER::init(const std::shared_ptr<message_t> &message)
{
	int _STATUS_ = 200;

	/////////////////////////////////////////////////////////////////////////////////////////  

	try
	{
		//
		// Инициализируем параметры
		//

		this->validator_init.validate(message->data);
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

		int k = 0;	

		while(work)
		{
			// TODO: Реализовать завершение цикла если необходимо остановить Платформу 

			std::cout << "fast cgi loop" << std::endl;

			auto msg = std::make_shared<message_http_t>();
			msg->_connection = new Connection_t();
			msg->apps = &this->apps;
	
			std::string uuid = tegia::random::uuid();
			std::string actor_name = "http/connection/" + uuid;
			
			FCGX_Init();
			FCGX_InitRequest(msg->_connection->req,  this->listen_socket, 0);
			rc = FCGX_Accept_r(msg->_connection->req);

			if (rc < 0)
			{
				//
				// TODO: обработать ошибку
				//

				std::cout << _ERR_TEXT_ << "ERROR REQUEST" << std::endl;
			}
			else
			{
				connections++;
				k++;
				tegia::message::send(actor_name, "/init", msg);
			}

			if(k == 1000)
			{
				std::cout << "connections = " << connections << std::endl;
				k = 0;
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

}	// END namespace HTTP
