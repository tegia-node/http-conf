#ifndef H_HTTP_MESSAGE
#define H_HTTP_MESSAGE

#include <tegia/actors/message_t.h>

#include "connection.h"
#include "application.h"

namespace HTTP {
class LISTENER;
class CONNECTION;
};

class message_http_t: public message_t
{
	friend class HTTP::LISTENER;
	friend class HTTP::CONNECTION;

	
	private:
		Connection_t * _connection = nullptr;
		applications_t * apps = nullptr;

	public:
		message_http_t(): message_t()
		{

		};

		~message_http_t()
		{

		};
};


#endif