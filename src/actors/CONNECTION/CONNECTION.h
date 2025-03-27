#ifndef H_HTTP_CONNECTION
#define H_HTTP_CONNECTION

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// INCLUDES                                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


//	TEGIA PLATFORM
	#include <tegia/tegia.h>
	#include <tegia/core/crypt.h>
	#include <tegia/core/cast.h>
	#include <tegia/app/auth.h>
	#include <tegia/core/string.h>
	#include <tegia/db/mysql/mysql.h>
	

//	COMMON 
	#include "../../common/params.h"
	#include "../../common/message_http.h"
	#include "../../common/connection.h"
	#include "../../common/domain.h"


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTOR HTTP::CONNECTION                                                                 //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


namespace HTTP {

class CONNECTION: public tegia::actors::actor_t
{	
	public:

		// ----------------------------------------------------------------------------------   
		// ACTION FUNCTIONS
		// ----------------------------------------------------------------------------------   

		CONNECTION(const std::string &name);

		~CONNECTION();  

		// static HTTP::CONNECTION * create(name,data);

		int init(const std::shared_ptr<message_t> &message);
		int response(const std::shared_ptr<message_t> &message);

		int test_01(const std::shared_ptr<message_t> &message);
		int test_02(const std::shared_ptr<message_t> &message);

	private:

		connection_t * connection = nullptr;
		domain_t * domain;

		tegia::json::validator _validator_http_data;

		// ----------------------------------------------------------------------------------   
		// SUPPORT FUNCTIONS 
		// ----------------------------------------------------------------------------------   

		int route_send(const std::shared_ptr<message_http_t> &message);
		int route_current(const std::shared_ptr<message_http_t> &message);
		int route_jwt_public_key(const std::shared_ptr<message_http_t> &message);

		int response_200(const std::shared_ptr<message_t> &message);

};

}	// END namespace HTTP


#endif 
