#include "application.h"
#include <tegia/context/auth.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	APPLICATION constructor
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////

application_t::application_t()
{

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	APPLICATION AUTH
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////


bool application_t::auth(Connection_t * connection)
{
	int status = 0;
	std::string token{};
	bool is_auth = true;

	if("Bearer " == connection->http_authorization.substr(0,7))
	{
		status = status + 1;
	}

	if(connection->cookie.find("tegia_token") != connection->cookie.end())
	{
		status = status + 10;
	}

	switch(status)
	{
		case 0:
		{
			// NO AUTH
			is_auth = false;
		}
		break;

		case 1:
		{
			token = connection->http_authorization.substr(7);
		}
		break;

		case 10:
		{
			token = connection->cookie["tegia_token"];
		}
		break;

		case 11:
		{
			// WTF???
			is_auth = false;
		}
		break;
	}

	if(is_auth == true)
	{
		std::cout << "token = " << token << std::endl;

		std::cout << "-----------------------------------" << std::endl; 
		std::cout << "UNPACK START" << std::endl;
		std::cout << "-----------------------------------" << std::endl; 

		std::string pub_key = tegia::auth::key("./jwt_keys/" + this->domain + "/jwtRS256.key.pub");
		auto jwt = tegia::auth::unpack(token,pub_key);

		std::cout << "state = " << jwt->status() << std::endl;
		jwt->print();

		std::cout << "-----------------------------------" << std::endl; 
		std::cout << "UNPACK END" << std::endl;
		std::cout << "-----------------------------------" << std::endl; 
	}

	return is_auth;
};