////////////////////////////////////////////////////////////////////////////////////////////
#undef _LOG_LEVEL_
#define _LOG_LEVEL_ _LOG_DEBUG_
#include <tegia/context/log.h>
////////////////////////////////////////////////////////////////////////////////////////////

#include "../CONNECTION.h"

////////////////////////////////////////////////////////////////////////////////////////////
/**

	\brief Обработчик сообщения /init
	\detail
	
*/   
////////////////////////////////////////////////////////////////////////////////////////////

namespace HTTP {

int CONNECTION::init(const std::shared_ptr<message_t> &message)
{
	int _STATUS_ = 200;
	LDEBUG("START CONNECTION " + this->name);

	auto msg = std::dynamic_pointer_cast<message_http_t>(message);
	if(!msg)
	{
		return 400;
	}

	//	
	//	PARSING HTTP PARAMS
	//

	this->connection = msg->_connection;
	this->connection->init(this->name);	

	//
	// FOUND DOMAIN
	//

	bool res;
	std::tie(res,this->domain) = msg->_domains->get(this->connection->server_name);
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
	// [ /api/message/send ]
	//

	if("/api/message/send" == this->connection->script_name)
	{
		return this->route_send(msg);
	}

	//
	// [ /api/current ]
	//

	if("/api/current" == this->connection->script_name)
	{
		return route_current(msg);
	}

	//
	// NOT FOUND
	//

	msg->http["response"]["status"] = 404;
	msg->http["response"]["type"] = "application/json";
	return 404;

	/////////////////////////////////////////////////////////////////////////////////////////  
};

}	// END namespace HTTP
