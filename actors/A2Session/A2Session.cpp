#include "A2Session.h"

#define _public_  true
#define _private_ false

#pragma comment(lib, "libfcgi.lib")

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// IMPORT                                                                                 //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#define ACTOR_TYPE "A2Session"

extern "C" tegia::actors::router_base * _load_actor()
{
	auto ro = new tegia::actors::router<A2Session>(ACTOR_TYPE);

	ACTION_ROUTE2( "/init",					&A2Session::init);
	ACTION_ROUTE2( "/authorize",			&A2Session::authorize);
	ACTION_ROUTE2( "/send",					&A2Session::send);

	ACTION_ROUTE2( "/test",					&A2Session::test);

	return ro;
};

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// CONSTRUCTOR / DESTRUCTOR                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


A2Session::A2Session(
	const std::string &name, 
	nlohmann::json data):tegia::actors::actor_base(ACTOR_TYPE, name, data)
{
	std::cout << _YELLOW_ << "CREATE ACTOR " << this->name << std::endl;

	this->request = new CRequest();

	//
	// PUBLIC JWT KEY
	//

	std::string pub_key_path = std::filesystem::current_path().string() + "/jwtRS256.key.pub";
	this->jwt_public_key = core::read_from_file(pub_key_path);

	//
	// HTTP ROUTERS
	//

	this->http_public_routes.insert("/api/v3/sessions/-/test");
	this->http_public_routes.insert("/api/v3/auth/vk/-/login/{script}");
	this->http_public_routes.insert("/api/v3/auth/vk/-/callback");
	
}; 

A2Session::~A2Session() 
{ 
	delete this->request;
};


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTION FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#include "actions/init.cpp"
#include "actions/send.cpp"
#include "actions/authorize.cpp"

#include "actions/test.cpp"

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// SUPPORT FUNCTIONS                                                                      //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////
