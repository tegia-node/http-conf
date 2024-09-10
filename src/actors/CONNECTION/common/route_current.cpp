#include "../CONNECTION.h"


namespace HTTP {

int CONNECTION::route_current(const std::shared_ptr<message_http_t> &message)
{
	this->domain->auth(this->connection,"");

	auto user = tegia::context::user();

	if(user->check(ROLES::SESSION::PUBLIC) == true)
	{
		std::cout << "[403]" << std::endl;
		std::cout << user->json() << std::endl;

		message->http["response"]["status"] = 403;
		message->http["response"]["type"] = "application/json";
		
		message->callback.add(this->name,"/response");
		return 403;
	}

	if(user->check(ROLES::SESSION::USER) == true)
	{
		std::cout << "[200]" << std::endl;
		std::cout << user->json() << std::endl;

		message->http["response"]["status"] = 200;
		message->http["response"]["type"] = "application/json";
		
		message->data = user->json();

		message->callback.add(this->name,"/response");
		return 200;
	}

	message->http["response"]["status"] = 400;
	message->http["response"]["type"] = "application/json";
	return 400;		
};

} // END namespace HTTP