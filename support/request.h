#ifndef _H_HTTP_REQUEST_
#define _H_HTTP_REQUEST_

//	VENDORS: FastCGI
	#include <unistd.h>
	#include <fcgi_stdio.h>
	#include <cgicc/CgiDefs.h>
	#include <cgicc/Cgicc.h>
	#include <cgicc/HTTPHTMLHeader.h>
	#include <cgicc/HTMLClasses.h>
	using namespace cgicc;
	// #include "../actors/A2FCGI/A2FCGI.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
// CRequest CLASS                                                                         //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////


class CRequest
{
	public:
		CRequest() 
		{   
			this->start_time = std::chrono::high_resolution_clock::now();
			this->req = new FCGX_Request;
		};

		~CRequest() 
		{
			delete this->req;

			this->end_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(this->end_time - this->start_time).count();

			std::cout 	<< _YELLOW_ << "[" << this->status << "] REQUEST " << _BASE_TEXT_ 
						<< this->url << " (time: " << duration << ")"  << std::endl;
		};

		int status = 0;
		std::string url;
		FCGX_Request *req;
		std::string content;

		std::chrono::high_resolution_clock::time_point start_time;
		std::chrono::high_resolution_clock::time_point end_time;
};


#endif 