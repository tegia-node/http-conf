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

	type->add_action("/init",         &HTTP::CONNECTION::init);
	type->add_action("/response",     &HTTP::CONNECTION::response);
	type->add_action("/test/01",      &HTTP::CONNECTION::test_01);
	type->add_action("/test/02",      &HTTP::CONNECTION::test_02);

	return type;
};


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
#include "actions/test.cpp"

	
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// COMMON FUNCTIONS                                                                       //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


