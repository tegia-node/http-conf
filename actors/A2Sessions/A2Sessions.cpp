////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// CONSTRUCTOR / DESTRUCTOR                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#include "A2Sessions.h"

#define _public_  true
#define _private_ false


A2Sessions::A2Sessions(
	const std::string &name, 
	nlohmann::json data):tegia::actors::actor_base(ACTOR_TYPE, name, data)
{
	//
	// ACTOR DATA
	//

	this->auth_server = data["auth_server"].get<std::string>();

	//
	// PUBLIC JWT KEY
	//

	std::string pub_key_path = std::filesystem::current_path().string() + "/jwtRS256.key.pub";
	this->jwt_public_key = core::read_from_file(pub_key_path);

	//
	// HTTP ROUTERS
	//

	this->http_public_routes.insert("/api/v3/auth/vk/-/login/add");
	this->http_public_routes.insert("/api/v3/auth/vk/-/login/{}/add");
	this->http_public_routes.insert("/api/v3/auth/vk/-/callback");

	this->http_public_routes.insert("/api/v3/sessions/-/init/{}");
	this->http_public_routes.insert("/api/v3/sessions/-/current");

	this->http_public_routes.insert("/api/v3/jwt/-/{}");
	this->http_public_routes.insert("/api/v3/jwt/-/generate/{}");

	this->http_public_routes.insert("/api/v3/auth/vk/-/confirm/{}");
	this->http_public_routes.insert("/api/v3/auth/vk/-/confirm/{}/add");

	this->http_public_routes.insert("/api/v3/auth/email/-/login/step1/add");
	this->http_public_routes.insert("/api/v3/auth/email/-/login/step2/add");


}; 

A2Sessions::~A2Sessions() { };


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTION FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#include "actions/current.cpp"
#include "actions/authorize.cpp"
#include "actions/init.cpp"

#include "actions/tests.cpp"

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// SUPPORT FUNCTIONS                                                                      //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////
