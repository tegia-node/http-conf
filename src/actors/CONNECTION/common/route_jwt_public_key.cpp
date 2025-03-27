#include "../CONNECTION.h"


namespace HTTP {

int CONNECTION::route_jwt_public_key(const std::shared_ptr<message_http_t> &message)
{
	this->domain->auth(this->connection,"");
	
	// std::cout << "[200]" << std::endl;
	// std::cout << user->json() << std::endl;

	message->http["response"]["status"] = 200;
	message->http["response"]["type"] = "application/json";

	std::string pub_key = tegia::auth::key("./jwt_keys/" + this->domain->name + "/jwtRS256.key.pub");
	message->data["key"] = pub_key;

	message->callback.add(this->name,"/response");
	return 200;
};

} // END namespace HTTP