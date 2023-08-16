#ifndef _H_HTTP_A2SESSIONS_
#define _H_HTTP_A2SESSIONS_

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
	
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTOR CLASS                                                                            //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#define ACTOR_TYPE "A2Sessions"

struct http_route
{
	bool is_public;
	std::string actor;
	std::string action;
};

class A2Sessions: public tegia::actors::actor_base
{	
	public:

		// ----------------------------------------------------------------------------------   
		// ACTION FUNCTIONS
		// ----------------------------------------------------------------------------------   

		A2Sessions(const std::string &name, nlohmann::json data);
		~A2Sessions();

		std::string authorize(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);
		std::string logout(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);
		std::string current(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);
		std::string init(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);

		std::string generate_jwt(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);

		std::string test1(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);
		std::string test2(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);


	protected:

		// ----------------------------------------------------------------------------------   
		// SUPPORT FUNCTIONS 
		// ----------------------------------------------------------------------------------   

		std::unordered_map<std::string, http_route> http_routes;
		std::unordered_set<std::string> http_public_routes;

		std::string jwt_public_key;
		std::string auth_server = "";

};

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// IMPORT                                                                                 //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


extern "C" tegia::actors::router_base * _load_actor()
{
	auto ro = new tegia::actors::router<A2Sessions>(ACTOR_TYPE);

	ACTION_ROUTE2( "/authorize",			&A2Sessions::authorize);
	ACTION_ROUTE2( "/logout",				&A2Sessions::logout);
	ACTION_ROUTE2( "/current",				&A2Sessions::current);

	ACTION_ROUTE2( "/init/{code}",			&A2Sessions::init);

	ACTION_ROUTE2( "/test1",				&A2Sessions::test1);
	ACTION_ROUTE2( "/test2",				&A2Sessions::test2);

	return ro;
};



#endif
