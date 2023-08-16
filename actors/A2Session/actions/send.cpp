
////////////////////////////////////////////////////////////////////////////////////////////
/**

*/
////////////////////////////////////////////////////////////////////////////////////////////

std::string A2Session::send(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{  
	int _STATUS_ = 0;
	/////////////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// Устанавливаем cookie для ответа
	//
	/////////////////////////////////////////////////////////////////////////////////////////

	std::string cookie = "";

	//
	// Определяем домен для cookie
	//

	std::string domain = message->http->request.server_name;
	auto arr = core::explode(domain,".");
	
	// domain = "." + arr[arr.size()-2] + "." + arr[arr.size()-1];
	domain = arr[arr.size()-2] + "." + arr[arr.size()-1];

	//
	// Генерируем cookie с jwt
	//

	/*
	cookie =	core::string::format() << cookie << "Set-Cookie: tegia_token=" 
				<< tegia::threads::current()->get_jwt()  << ";"
				<< " path=/; Max-Age=" << this->maxage << "; Domain=" << domain << "; SameSite=Strict; HttpOnly\r\n";
	*/

	//
	// https://web.dev/i18n/ru/samesite-cookies-explained/
	//

	cookie =	core::string::format() << cookie << "Set-Cookie: tegia_token=" 
				<< tegia::threads::current()->get_jwt()  << ";"
				<< " path=/; Max-Age=" << this->maxage << "; Domain=" << domain << "; SameSite=Lax; HttpOnly\r\n";

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// Настраиваем заголовки для CORS
	//
	/////////////////////////////////////////////////////////////////////////////////////////

	if(this->alloworigin == true)
	{
		std::string origin = "http://dit.local";
			
		if(message->http->request.http_origin != "")
		{
			origin = message->http->request.http_origin;
		}
		else
		{
			origin = message->http->request.server_name;
			origin = "http://" + origin;
		}

		std::string header = message->data["responce"]["header"].get<std::string>();   
		header = header + "Access-Control-Allow-Origin: " + origin + "\r\n";
		header = header + "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
		header = header + "Access-Control-Allow-Credentials: true\r\n";
		header = header + "Access-Control-Allow-Headers: DNT,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Range\r\n";
		message->data["responce"]["header"] = header;
	}


	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//  Формируем ответы в зависимости от статуса ответа
	//
	/////////////////////////////////////////////////////////////////////////////////////////

	std::string type = message->data["responce"]["type"].get<std::string>();

	if("302" == type)
	{
		request->status = 302;
		request->content = core::string::format() << cookie 
				<< "Status: 302 Found\r\n" 
				<< message->data["responce"]["cache"].get<std::string>() 
				<< message->data["responce"]["header"].get<std::string>()
				<< "Content-Type: text/javascript;charset=UTF-8\r\n"
				<< "Location: " <<  message->data["responce"]["location"].get<std::string>()
				<< "\r\n\r\n";
		
		FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);
		FCGX_Finish_r(request->req); //Завершаем запрос

		//delete request;
		return "302";
	} 


	if("303" == type)
	{
		request->status = 303;
		request->content = core::string::format() << cookie 
				<< "Status: 303 See Other\r\n" 
				<< message->data["responce"]["cache"].get<std::string>() 
				<< message->data["responce"]["header"].get<std::string>()
				<< "Content-Type: text/javascript;charset=UTF-8\r\n"
				<< "Location: " <<  message->data["responce"]["location"].get<std::string>()
				<< "\r\n\r\n";
  
		FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);
		FCGX_Finish_r(request->req);//Завершаем запрос

		//delete request;
		return "303";		
	} 


	if("400" == type)
	{
		request->status = 400;
		request->content = core::string::format() << cookie 
				<< "Status: 400 Bad Request\r\n" 
				<< "Cache-Control: no-cache\r\n" 
				<< message->data["responce"]["header"].get<std::string>()
				<< "Content-Type: application/json; charset=utf-8\r\n"
				<< core::cast<std::string>(message->data["responce"]["message"])
				<< "\r\n\r\n";
		 
		FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);
		FCGX_Finish_r(request->req);//Завершаем запрос

		//delete request;
		return "400";
	} 


	if("401" == type)
	{
		request->status = 401;
		request->content = core::string::format() << cookie 
				<< "Status: 401 Unauthorized\r\n" 
				<< "Cache-Control: no-cache\r\n" 
				<< message->data["responce"]["header"].get<std::string>()
				<< "Content-Type: application/json; charset=utf-8\r\n"
				<< "{ \"status\": \"error\", \"error\": {\"code\": 401,\"message\": \"unauthorized\"}}"
				<< "\r\n\r\n";
		 
		FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);
		FCGX_Finish_r(request->req);//Завершаем запрос

		//delete request;
		return "401";
	} 


	if("404" == type)
	{
		request->content = core::string::format() << cookie 
				<< "Status: 404 Not Found\r\n" 
				<< "Cache-Control: no-cache\r\n" 
				<< message->data["responce"]["header"].get<std::string>()
				<< "Content-Type: application/json; charset=utf-8\r\n"
				<< "{ \"status\": \"error\",\"error\": {\"code\": 404,\"message\": \"Not Found\"} }"
				<< "\r\n\r\n";
		 
		FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);
		FCGX_Finish_r(request->req);//Завершаем запрос

		//delete request;
		return "404";
	} 


	if("string" == type)
	{
		request->content = core::string::format() << cookie 
				<< message->data["responce"]["status"].get<std::string>() 
				<< message->data["responce"]["cache"].get<std::string>() 
				<< message->data["responce"]["header"].get<std::string>() 
				<< "\r\n"
				<< message->data["responce"]["data"].get<std::string>()
				<< "\r\n\r\n";                           
		 
		FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);
		FCGX_Finish_r(request->req);//Завершаем запрос

		//delete request;
		return "string";
	} 


	if("json" == type)
	{
		try
		{
			request->status = 200;
			request->content = cookie + 
				message->data["responce"]["status"].get<std::string>() +
				message->data["responce"]["cache"].get<std::string>() +
				message->data["responce"]["header"].get<std::string>() +
				"\r\n" +
				message->data["responce"]["data"].dump() +
				"\r\n\r\n";					
		
			FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);
			FCGX_Finish_r(request->req);  //Завершаем запрос

			//delete request;
			return "200";
		}

		catch (nlohmann::json::exception &e)
		{
			request->status = 500;
			request->content = cookie + 
				"Status: 500 Internal Server Error\r\n"
				"Cache-Control: no-cache\r\n" + 
				message->data["responce"]["header"].get<std::string>() +
				"Content-Type: application/json; charset=utf-8\r\n"
				"\r\n\r\n";

			FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);
			FCGX_Finish_r(request->req);  //Завершаем запрос

			delete request;
			return "500";
		}          
	} 
	// end if("json" == type)


	if("file" == type)
	{
		// Читаем файл с диска   

		// TODO: было бы полезно сделать файловый буфер - читать самые часто используемые файлы в оперативную память
		 
		std::string path = message->data["responce"]["data"]["data"]["path"].get<std::string>();
		std::string filecontent;

		// TODO: Проверить, что такой файл существует + реализовать отдачу файла по мере чтения с диска
	 
		std::ifstream file(path.c_str(), std::ios::binary); 
		char c;
		while(file.get(c))
		{
			filecontent += c;
		};
  
		file.close();

		// Формируем ответ
		request->content = core::string::format() << message->data["responce"]["status"].get<std::string>() 
								<< message->data["responce"]["cache"].get<std::string>()
								<< "Content-type: " << message->data["responce"]["data"]["data"]["type"].get<std::string>() 
								<< "; charset=utf-8\r\nContent-Length: " << filecontent.length() 
								<< "\r\nContent-Disposition: attachment; filename=\"" 
								<< message->data["responce"]["data"]["data"]["filename"].get<std::string>() 
								<< "\"\r\n\r\n" << filecontent;
		FCGX_PutStr(request->content.c_str(), request->content.size(),request->req->out);                          
		FCGX_Finish_r(request->req);//Завершаем запрос

		if(message->data["responce"]["tmp"].is_null() == false && message->data["responce"]["tmp"].get<bool>() == true)
		{
			std::remove(path.c_str());
		}

		//delete request;
		return "file";		

	}
	// end if("file" == type)

	// message->callback.push({"fcgi","/end/" + this->uuid})
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//delete request;
	return "ok";
};


