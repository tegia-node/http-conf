#ifndef H_HTTP_LISTENER
#define H_HTTP_LISTENER

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// INCLUDES                                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


//	TEGIA PLATFORM
	#include <tegia/tegia.h>
	#include <tegia/core/crypt.h>
	

//	COMMON 
	#include "../../common/params.h"
	#include "../../common/message_http.h"
	#include "../../common/connection.h"
	#include "../../common/domain.h"
	#include "../../common/ws.h"


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTOR HTTP::LISTENER                                                                   //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


namespace HTTP {

class LISTENER: public tegia::actors::actor_t
{	
	public:

		LISTENER(const std::string &name);
		~LISTENER();  

		// ----------------------------------------------------------------------------------   
		// ACTION FUNCTIONS
		// ----------------------------------------------------------------------------------   

		int init(const std::shared_ptr<message_t> &message);
		int add_domain(const std::shared_ptr<message_t> &message);

	private:

		tegia::json::validator _validator_init;
		int listen_socket;

		domains_t domains;
		workspaces_t workspaces;

		long long int connections = 0;

		// ----------------------------------------------------------------------------------   
		//
		// SUPPORT FUNCTIONS 
		//
		// ----------------------------------------------------------------------------------   

		bool init_port(_params * params);

};

}	// END namespace HTTP


#endif 
