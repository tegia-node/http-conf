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

class CONNECTION: public tegia::actors::actor_t<HTTP::CONNECTION>
{	
	public:

		// ----------------------------------------------------------------------------------   
		// ACTION FUNCTIONS
		// ----------------------------------------------------------------------------------   

		CONNECTION(
			const std::string &name, 
			tegia::actors::type_t<HTTP::CONNECTION> * type);

		~CONNECTION();  

		// static HTTP::CONNECTION * create(name,data);

		int init(const std::shared_ptr<message_t> &message);
		int response(const std::shared_ptr<message_t> &message);

		int test_01(const std::shared_ptr<message_t> &message);
		int test_02(const std::shared_ptr<message_t> &message);

	private:

		connection_t * connection = nullptr;
		domain_t * domain;

		// ----------------------------------------------------------------------------------   
		// SUPPORT FUNCTIONS 
		// ----------------------------------------------------------------------------------   

		int response_200(const std::shared_ptr<message_t> &message);

};

}	// END namespace HTTP


#endif 
