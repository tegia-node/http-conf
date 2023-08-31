#ifndef H_HTTP_PARAMS
#define H_HTTP_PARAMS

#include <string>

struct _fcgi
{
	std::string host;
	int listen_queue_backlog;
};

struct _cookie
{
	long long int maxage;	// Время жизни cookie, выставляемое при отправке http-запроса клиенту
	std::string same_site;	//	
	bool http_only;
};

struct _cors
{
	bool alloworigin;		// Флаг, определяющий формировать ли заголовки для CORS
};

struct _params
{
	_fcgi fcgi;
	_cookie cookie;
	_cors cors;
};


#endif 