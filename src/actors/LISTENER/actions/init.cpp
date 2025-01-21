#include "../LISTENER.h"

#undef _LOG_LEVEL_
#define _LOG_LEVEL_ _LOG_NOTICE_
#include <tegia/context/log.h>

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

	//
	// Инициализируем параметры
	//

	auto res = this->_validator_init.validate(message->data);
	if(res == false)
	{
		// std::cout << _ERR_TEXT_ "NOT VALID" << std::endl;
		// std::cout << message->data << std::endl;
		return 500;	
	}

	_params * params = new _params();

	// fcgi
	params->fcgi.host = message->data["fcgi"]["host"].get<std::string>();
	params->fcgi.listen_queue_backlog = message->data["fcgi"]["listen_queue_backlog"].get<int>();

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

	try
	{
		int rc = 0;
		std::mutex accept_mutex;
		bool work = true;	

		while(work)
		{
			/*
				TODO: Реализовать завершение цикла если необходимо остановить Платформу 
			*/ 

			auto msg = std::make_shared<message_http_t>();
			msg->_connection = new connection_t();
			msg->_domains = &this->domains;
			msg->_workspaces = &this->workspaces;
	
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
				tegia::message::send(actor_name, "/init", msg);
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

	delete params;

	/////////////////////////////////////////////////////////////////////////////////////////  
	return _STATUS_;
};

}	// END namespace HTTP
