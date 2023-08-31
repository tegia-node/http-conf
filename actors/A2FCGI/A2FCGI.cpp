#include "A2FCGI.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// IMPORT                                                                                 //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#define ACTOR_TYPE "A2FCGI"

extern "C" tegia::actors::router_base * _load_actor()
{
	auto ro = new tegia::actors::router<A2FCGI>(ACTOR_TYPE);

	ACTION_ROUTE2( "/loop",						&A2FCGI::loop);
	ACTION_ROUTE2( "/end/{uuid}",				&A2FCGI::end);
	//ACTION_ROUTE2( "/http/send",				&A2FCGI::http_send);

	return ro;
};



////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// PROVIDE FUNCTION                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "libfcgi.lib")

//#define DEVELOPER_VERSION

#if defined(DEVELOPER_VERSION)
#define DOUT(text) std::cout << text << std::endl;     
#else
#define DOUT(text)     
#endif

// https://github.com/toshic/libfcgi/blob/master/examples/echo-cpp.cpp

static const unsigned long STDIN_MAX = 1000000;


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// CONSTRUCTOR / DESTRUCTOR                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

A2FCGI::A2FCGI(
	const std::string &name, 
	nlohmann::json &data):tegia::actors::actor_base(ACTOR_TYPE, name, data)
{
	//
	// Валидируем конфигурационные данные
	//

	try 
	{
		this->validator.set_root_schema(
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
							"port":
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
		std::cout << data.dump() << std::endl;
		exit(0);
	}	
}; 

A2FCGI::~A2FCGI() { };

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTION FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#include "actions/end.cpp"
#include "actions/loop.cpp"
//#include "actions/http_send.cpp"

	
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// SUPPORT FUNCTIONS                                                                      //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


bool A2FCGI::init_port(_params * params)
{
	LDEBUG( "Пытаемся инициализировать порт " << port );

	if(FCGX_Init())
	{    
		LERROR("Ошибка инициализации библиотеки fastcgi");
		return false; 
	}

	this->listen_socket = FCGX_OpenSocket(params->fcgi.host.c_str(), params->fcgi.listen_queue_backlog); //Открываем новый слушающий сокет
	if(this->listen_socket < 0)
	{
		LERROR("Ошибка инициализации порта " << params->fcgi.host);
		return false; 
	}

	LNOTICE("Порт " << params->fcgi.host << " успешно инициализирован" );
	return true;
};    



