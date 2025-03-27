#include "domain.h"
#include <tegia/app/auth.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	APPLICATION constructor
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////

domain_t::domain_t()
{

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	APPLICATION AUTH
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////


bool domain_t::auth(connection_t * connection, const std::string &wsid)
{
	int status = 0;
	std::string token{};
	bool is_auth = true;

	// std::cout << _YELLOW_ << connection->http_authorization << _BASE_TEXT_ << std::endl;
	
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
			token = "";
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
		// std::cout << "-----------------------------------" << std::endl; 
		// std::cout << "UNPACK START" << std::endl;
		// std::cout << "-----------------------------------" << std::endl; 

		// std::cout << "domain = " << this->name << std::endl;
		// std::cout << "token  = " << token << std::endl;

		std::string pub_key = tegia::auth::key("./jwt_keys/" + this->name + "/jwtRS256.key.pub");
		auto jwt = tegia::auth::unpack(token,pub_key);

		// std::cout << "state = " << jwt->status() << std::endl;
		// std::cout << "tid = " << tegia::context::tid() << std::endl;
		// jwt->print();
		// tegia::threads::user()->print();
		
		// std::cout << "-----------------------------------" << std::endl; 
		// std::cout << "UNPACK END" << std::endl;
		// std::cout << "-----------------------------------" << std::endl; 
	}

	return is_auth;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	APPLICATION AUTH
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////


std::string jwt_public_key()
{
	return "";
};