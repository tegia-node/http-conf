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

bool LISTENER::init_port(_params * params)
{
	LDEBUG( "Пытаемся инициализировать порт " + params->fcgi.host);

	if(FCGX_Init())
	{    
		LERROR("Ошибка инициализации библиотеки fastcgi");
		return false; 
	}

	this->listen_socket = FCGX_OpenSocket(params->fcgi.host.c_str(), params->fcgi.listen_queue_backlog); //Открываем новый слушающий сокет
	if(this->listen_socket < 0)
	{
		LERROR("Ошибка инициализации порта " + params->fcgi.host);
		return false; 
	}

	LNOTICE("Порт " + params->fcgi.host + " успешно инициализирован" );
	return true;
}; 

}	// END namespace HTTP

