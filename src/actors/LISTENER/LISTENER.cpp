#include "LISTENER.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// IMPORT                                                                                 //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#define ACTOR_TYPE "HTTP::LISTENER"


extern "C" tegia::actors::type_base_t * _init_type(const std::string &type_name)
{	
	auto type = new tegia::actors::type_t<HTTP::LISTENER>(ACTOR_TYPE);

	type->add_action("/parse",   &HTTP::LISTENER::init);
	type->add_action("/run",     &HTTP::LISTENER::add_application);

	RETURN_TYPE(type,HTTP::LISTENER)
};


/*
extern "C" tegia::actors::type_base * _load_type()
{
	auto actor_type = new tegia::actors::type<HTTP::LISTENER>(ACTOR_TYPE);

	ADD_ACTION( "/init",							&HTTP::LISTENER::init);
	ADD_ACTION( "/application/add",					&HTTP::LISTENER::add_application);

	return actor_type;
};
*/



////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// PROVIDE FUNCTION                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "libfcgi.lib")

// https://github.com/toshic/libfcgi/blob/master/examples/echo-cpp.cpp

static const unsigned long STDIN_MAX = 1000000;


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// CONSTRUCTOR / DESTRUCTOR                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

namespace HTTP {


LISTENER::LISTENER(
	const std::string &name, 
	tegia::actors::type_t<HTTP::LISTENER> * type)
: tegia::actors::actor_t<HTTP::LISTENER>(name,type)
{
	//
	// Инициализируем JSON SCHEME VALIDATOR для /init
	//

	try 
	{
		this->validator_init.set_root_schema(
			R"({
				"$schema": "http://json-schema.org/draft-07/schema#",
				"title": "HTTP config schema",
				"type": "object",
				"properties": 
				{
					"fcgi":
					{
						"type": "object",
						"properties":
						{
							"host":
							{
								"type": "string",
								"description": "Порт, на который приходит fcgi соединение от nginx"
							}
						}
					},
					"cors":
					{
						"type": "object",
						"properties":
						{
							"alloworigin":
							{
								"type": "boolean",
								"description": "Флаг, определяющий формировать ли заголовки для CORS"
							}
						},
						"required": ["alloworigin"]
					},
					"cookie":
					{
						"type": "object",
						"properties":
						{
							"maxage":
							{
								"type": "number",
								"description": "Время жизни cookie, выставляемое при отправке http-запроса клиенту"
							}
						},
						"required": ["maxage"]
					}
				},
				"required": ["cors","cookie"]
			})"_json
		);
	}

	catch (const std::exception &e)
	{
		std::cout << _ERR_TEXT_ << " Validation config error: " << e.what() << std::endl;
		// std::cout << data.dump() << std::endl;
		exit(0);
	}	
};

LISTENER::~LISTENER() { };

}	// END namespace HTTP


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTION FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


#include "actions/init.cpp"
#include "actions/add_application.cpp"

	
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// COMMON FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


#include "support/init_port.cpp"


