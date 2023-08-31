#ifndef _H_HTTP_A2SESSION_
#define _H_HTTP_A2SESSION_

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// INCLUDES                                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

//	STL
	#include <unordered_map>
	#include <unordered_set>
	#include <filesystem>

//	TEGIA PLATFORM
	#include <tegia/tegia.h>
	#include <tegia/context/jwt.h>
	#include <tegia/actors/routers_map.h>
	#include <tegia/actors/actor_base.h>
	#include <tegia/context/thread_context.h>

	#include <tegia/core/core.h>
	#include <tegia/core/http.h>


	#include "../../support/request.h"
	#include "../../support/params.h"
	#include "../../support/message_http.h"	
	
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTOR CLASS                                                                            //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


struct http_route
{
	bool is_public;
	std::string actor;
	std::string action;
};

class A2FCGI;

class A2Session: public tegia::actors::actor_base
{	
	friend class A2FCGI;
	
	public:

		// ----------------------------------------------------------------------------------   
		// ACTION FUNCTIONS
		// ----------------------------------------------------------------------------------   

		A2Session(const std::string &name, nlohmann::json data);
		~A2Session();

		std::string init(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);
		std::string send(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);
		std::string authorize(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);

		std::string test(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);

	protected:

		CRequest *request;
		// Переменные окружения, определенные в конфигурационном файле

		//int port;							// Прослушиваемый порт для приема входящих fastcgi-соединений
		//long int maxage;					// Время жизни cookie, выставляемое при отправке http-запроса клиенту
		//bool alloworigin;					// Флаг, определяющий формировать ли заголовки для CORS
		_params * params;
		std::string uuid;

		// ----------------------------------------------------------------------------------   
		// SUPPORT FUNCTIONS 
		// ----------------------------------------------------------------------------------   

		std::unordered_map<std::string, http_route> http_routes;
		std::unordered_set<std::string> http_public_routes;
		std::string jwt_public_key;

};


#endif
