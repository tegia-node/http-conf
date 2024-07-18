#include "CONNECTION.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// IMPORT                                                                                 //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

#define ACTOR_TYPE "HTTP::CONNECTION"


extern "C" tegia::actors::type_base_t * _init_type(const std::string &type_name)
{	
	auto type = new tegia::actors::type_t<HTTP::CONNECTION>(ACTOR_TYPE);

	type->add_action("/parse",   &HTTP::CONNECTION::init);
	type->add_action("/run",     &HTTP::CONNECTION::response);

	RETURN_TYPE(type,HTTP::CONNECTION)
};


/*
extern "C" tegia::actors::type_base * _load_type()
{
	auto actor_type = new tegia::actors::type<HTTP::CONNECTION>(ACTOR_TYPE);

	ADD_ACTION( "/init",							&HTTP::CONNECTION::init);
	ADD_ACTION( "/response",						&HTTP::CONNECTION::response);

	return actor_type;
};
*/



////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// CONSTRUCTOR / DESTRUCTOR                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

namespace HTTP {


CONNECTION::CONNECTION(
	const std::string &name, 
	tegia::actors::type_t<HTTP::CONNECTION> * type)
: tegia::actors::actor_t<HTTP::CONNECTION>(name,type)
{
	
};

CONNECTION::~CONNECTION() { };

}	// END namespace HTTP


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTION FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


#include "actions/init.cpp"
#include "actions/response.cpp"

	
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// COMMON FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


