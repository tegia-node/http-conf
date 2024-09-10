#include "../CONNECTION.h"


namespace HTTP {

int CONNECTION::route_send(const std::shared_ptr<message_http_t> &message)
{
	//
	// VALIDATE DATA
	//

	auto res = this->_validator_http_data.validate(this->connection->post);
	if(res == false)
	{
		std::cout << _ERR_TEXT_ "NOT VALID" << std::endl;
		std::cout << this->connection->post << std::endl;

		message->http["response"]["status"] = 400;
		message->http["response"]["type"] = "application/json";

		return 400;	
	}

	//
	// AUTH
	//

	this->domain->auth(this->connection,"");

	//
	// FORWARD
	//

	auto _msg = tegia::message::init();
	_msg->http["response"]["status"] = 200;
	_msg->http["response"]["type"] = "application/json";
	_msg->http["request"] = this->connection->json();
	_msg->http["action"] = this->connection->post["route"].get<std::string>();
	_msg->http["application"]["name"] = domain->name;
	_msg->callback.add(this->name,"/response");

	tegia::message::send(
		this->connection->post["ws"].get<std::string>(),
		"/router",
		_msg);

	return 200;
};

} // END namespace HTTP