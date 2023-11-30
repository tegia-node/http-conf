#ifndef H_HTTP_CONNECTION
#define H_HTTP_CONNECTION

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// INCLUDES                                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


//	TEGIA PLATFORM
	#include <tegia2/tegia.h>
	#include <tegia2/core/crypt.h>
	#include <tegia/core/cast.h>
	

//	COMMON 
	#include "../../common/params.h"
	#include "../../common/message_http.h"
	#include "../../common/connection.h"
	#include "../../common/application.h"

	#include "common/http_request.h"


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTOR HTTP::CONNECTION                                                                 //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


namespace HTTP {

class CONNECTION: public tegia::actors::actor_base
{	
	public:

		// ----------------------------------------------------------------------------------   
		// ACTION FUNCTIONS
		// ----------------------------------------------------------------------------------   

		CONNECTION(const std::string &name, nlohmann::json &data); 
		~CONNECTION();  

		int init(const std::shared_ptr<message_t> &message);
		int response(const std::shared_ptr<message_t> &message);

	private:

		Connection_t * connection = nullptr;
		application_t * app;

		// ----------------------------------------------------------------------------------   
		// SUPPORT FUNCTIONS 
		// ----------------------------------------------------------------------------------   

		int response_200(const std::shared_ptr<message_t> &message);

};

}	// END namespace HTTP


#endif 
