#ifndef H_HTTP_MESSAGE
#define H_HTTP_MESSAGE

#include <tegia/actors/message_t.h>

#include "connection.h"
#include "domain.h"
#include "ws.h" 

namespace HTTP {
class LISTENER;
class CONNECTION;
};

class message_http_t: public message_t
{
	friend class HTTP::LISTENER;
	friend class HTTP::CONNECTION;

	
	private:
		connection_t * _connection = nullptr;
		domains_t    * _domains = nullptr;
		workspaces_t * _workspaces = nullptr;

	public:
		message_http_t(): message_t()
		{

		};

		~message_http_t()
		{

		};
};


#endif