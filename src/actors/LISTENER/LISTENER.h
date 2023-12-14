#ifndef H_HTTP_LISTENER
#define H_HTTP_LISTENER

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// INCLUDES                                                                               //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


//	TEGIA PLATFORM
	#include <tegia2/tegia.h>
	#include <tegia2/core/crypt.h>
	

//	COMMON 
	#include "../../common/params.h"
	#include "../../common/message_http.h"
	#include "../../common/connection.h"
	#include "../../common/application.h"


////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTOR HTTP::LISTENER                                                                   //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

namespace HTTP {

class LISTENER: public tegia::actors::actor_base
{	
	public:

		// ----------------------------------------------------------------------------------   
		//
		// ACTION FUNCTIONS
		//
		// ----------------------------------------------------------------------------------   

		LISTENER(const std::string &name, nlohmann::json &data); 
		~LISTENER();  

		int init(const std::shared_ptr<message_t> &message);
		int add_application(const std::shared_ptr<message_t> &message);

	private:

		nlohmann::json_schema::json_validator validator_init;
		int listen_socket;

		// хост и порт, которые слушает актор
		std::string host;

		// максимальное число подключенных соединений
		int listen_queue_backlog = 0;

		// Множество приложений, которые "обслуживаются" этим актором
		applications_t apps;

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
