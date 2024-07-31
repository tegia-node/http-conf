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

class custom_error_handler : public nlohmann::json_schema::basic_error_handler
{
	void error(const nlohmann::json::json_pointer &pointer, const nlohmann::json &instance,
		const std::string &message) override
	{
		nlohmann::json_schema::basic_error_handler::error(pointer, instance, message);

		std::cout << _ERR_TEXT_ << "json validate error" << "\n";
		std::cout << "pointer  = " << pointer  << "\n";
		std::cout << "instance = " << instance << "\n";
		std::cout << "message  = " << message  << "\n";
	};
};


class validator
{
	public:
		bool load(const std::string &filename)
		{
			int res = 0;
			std::string info = "";
			std::tie(res,info,this->_schema) = tegia::json::_file(filename);

			if(res != 0)
			{
				return false;
			}

			this->_validator.set_root_schema(this->_schema); 
			this->_is_init = true;
			return true;
		};

		bool load(nlohmann::json schema)
		{
			this->_schema = schema;
			this->_validator.set_root_schema(this->_schema); 
			this->_is_init = true;
			return true;
		};

		bool validate(nlohmann::json data)
		{
			if(_is_init == false) return false;
		
			this->_validator.validate(data, this->_err);

			if (this->_err)
			{
				// std::cerr << "Validation failed\n";
				return false;
			}
			else
			{
				// std::cout << "Validation succeeded\n";
				return true;
			}
		};

	private:
		nlohmann::json _schema;
		nlohmann::json_schema::json_validator _validator;
		custom_error_handler _err;

		bool _is_init = false;
};


namespace HTTP {

class LISTENER: public tegia::actors::actor_t<HTTP::LISTENER>
{	
	public:

		// ----------------------------------------------------------------------------------   
		//
		// ACTION FUNCTIONS
		//
		// ----------------------------------------------------------------------------------   

		LISTENER(
			const std::string &name, 
			tegia::actors::type_t<HTTP::LISTENER> * type);

		~LISTENER();  


		int init(const std::shared_ptr<message_t> &message);
		int add_domain(const std::shared_ptr<message_t> &message);

	private:

		validator _validator_init;
		int listen_socket;
		// std::string host;
		// int listen_queue_backlog = 0;

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
