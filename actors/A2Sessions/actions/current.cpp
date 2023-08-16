
////////////////////////////////////////////////////////////////////////////////////////////
/**

*/
////////////////////////////////////////////////////////////////////////////////////////////

std::string A2Sessions::current(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{  
	auto responcedata = &(message->data["responce"]["data"]);
	/////////////////////////////////////////////////////////////////////////////////////

	//std::cout << tegia::threads::current()->context->jwt.json() << std::endl;
	
	std::this_thread::sleep_for(2s);

	if(tegia::threads::current()->context->jwt.get_status() == 0)
	{
		//message->data["responce"]["location"] = "/";
		message->data["responce"]["type"] = "401";
	}
	else
	{
		(*responcedata)["status"] = "ok";
		(*responcedata)["data"]["current_user"] = tegia::threads::current()->context->jwt.json();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	return "ok";
};



std::string A2Sessions::logout(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{
	auto responcedata = &(message->data["responce"]["data"]);
	/////////////////////////////////////////////////////////////////////////////////////

	tegia::threads::current()->context = std::make_shared<tegia::threads::context>();

	std::cout << "current thread  = " << tegia::threads::current()->json() << std::endl;
	std::cout << "current user_uuid = " << tegia::threads::user_uuid() << std::endl;
	
	/////////////////////////////////////////////////////////////////////////////////////
	return "ok";
};



////////////////////////////////////////////////////////////////////////////////////////////
/**

*/
////////////////////////////////////////////////////////////////////////////////////////////

std::string A2Sessions::generate_jwt(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{
	/////////////////////////////////////////////////////////////////////////////////////

	std::string priv_key_path = std::filesystem::current_path().string() + "/jwtRS256.key";
	std::string priv_key = core::read_from_file(priv_key_path);

	std::cout << "current thread  = " << tegia::threads::current()->json() << std::endl;
	std::cout << "current user_uuid = " << tegia::threads::user_uuid() << std::endl;


	/*
	JWT jwt;
	jwt._id = 23;
	jwt._t_create = "2010-01-01";
	jwt.status = 1;

	jwt.inn = "690501263078";
	jwt.name = "Игорь";
	jwt.surname = "Горячев";
	jwt.patronymic = "Александрович";
	jwt.photo = "photo";
	jwt.sex = 1;
	jwt.roles = nlohmann::json();

	jwt.type = 1;
	jwt.ip = "127.0.0.1";

	if(jwt.pack(priv_key) == 0)
	{
		std::cout << _OK_TEXT_ << " geterate jwt = [" << jwt.token << "]" << std::endl;
	}
	else
	{
		std::cout << _ERR_TEXT_ << " geterate jwt" << std::endl;
	}
	*/

	/////////////////////////////////////////////////////////////////////////////////////
	return "success2";
};
