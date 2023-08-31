#ifndef _H_HTTP_A2FCGI_
#define _H_HTTP_A2FCGI_

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// INCLUDES                                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

//	C++ STL
	#include <memory>
	#include <stdlib.h>
	#include <chrono> 
	#include <fstream>
	#include <thread>

//	TEGIA PLATFORM
	#include <tegia/tegia.h>
	#include <tegia/actors/routers_map.h>
	#include <tegia/actors/actor_base.h>

	#include <tegia/core/core.h> 
	#include <tegia/core/crypt.h>
	

//	VENDORS 
	#include <nlohmann/json-schema.hpp>

	#include "../../support/request.h"
	#include "../../support/params.h"
	// #include "../../support/message_http.h"

	#include "../A2Session/A2Session.h"
	


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTOR A2FCGI CLASS                                                                     //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


class A2FCGI: public tegia::actors::actor_base
{	
	public:

		// ----------------------------------------------------------------------------------   
		// ACTION FUNCTIONS
		// ----------------------------------------------------------------------------------   

		A2FCGI(const std::string &name, nlohmann::json &data); 
		~A2FCGI();  

		std::string end(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);
		std::string loop(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params);

	private:

		nlohmann::json_schema::json_validator validator;
		int listen_socket;

		// ----------------------------------------------------------------------------------   
		// SUPPORT FUNCTIONS 
		// ----------------------------------------------------------------------------------   

		bool init_port(_params * params);
};



#endif 
