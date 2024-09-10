#ifndef H_HTTP_WS
#define H_HTTP_WS

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

	#include <tegia/ws/ws.h>
	

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// ACTOR HTTP::WS                                                                         //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


namespace HTTP {

class WS: public tegia::actors::ws_t
{	
	public:

		// ----------------------------------------------------------------------------------   
		// ACTION FUNCTIONS
		// ----------------------------------------------------------------------------------   

		WS(const std::string &name);

		~WS();  

		int add(const std::shared_ptr<message_t> &message);
		int resolve(const std::shared_ptr<message_t> &message);

	private:

		// ----------------------------------------------------------------------------------   
		// SUPPORT FUNCTIONS 
		// ----------------------------------------------------------------------------------   

};

}	// END namespace WS


#endif 
