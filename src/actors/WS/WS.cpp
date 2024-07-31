#include "WS.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// IMPORT                                                                                 //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#define ACTOR_TYPE "HTTP::WS"


template <>
tegia::actors::actor_base_t *  tegia::actors::type_t<HTTP::WS>::create_actor(const std::string &name)
{
	std::cout << "create actor HTTP::WS" << std::endl;

	exit(0);
	
	return new HTTP::WS(name, this);
};




extern "C" tegia::actors::type_base_t * _init_type(const std::string &type_name)
{	
	auto type = new tegia::actors::type_t<HTTP::WS>(ACTOR_TYPE);

	type->add_action("/resolve",         &HTTP::WS::resolve);

	return type;
};


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// CONSTRUCTOR / DESTRUCTOR                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

namespace HTTP {


WS::WS(
	const std::string &name, 
	tegia::actors::type_t<HTTP::WS> * type)
: tegia::actors::actor_t<HTTP::WS>(name,type)
{
	
};

WS::~WS() { };

}	// END namespace HTTP


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTION FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


#include "actions/resolve.cpp"

	
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// COMMON FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


