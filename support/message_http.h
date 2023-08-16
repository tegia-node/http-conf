#ifndef _H_HTTP_MESSAGE_
#define _H_HTTP_MESSAGE_

class A2FCGI;

class message_http: public message_t
{
	friend class A2FCGI;
	
	private:
		// CRequest * _request;

	public:
		message_http(nlohmann::json json_data): message_t(json_data)
		{
			//this->_request = new CRequest();
		};

		~message_http()
		{
			//delete this->_request;
		};
};


#endif