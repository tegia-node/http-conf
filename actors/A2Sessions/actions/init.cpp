
////////////////////////////////////////////////////////////////////////////////////////////
/**

*/
////////////////////////////////////////////////////////////////////////////////////////////

std::string A2Sessions::init(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{  
	auto responcedata = &(message->data["responce"]["data"]);
	/////////////////////////////////////////////////////////////////////////////////////

	auto http = new tegia::http::client();
	std::string code = route_params["code"].get<std::string>();

	int status = http->get(this->auth_server + "/api/v3/jwt/-/" + code);
	switch(status)
	{
		case 200:
		{
			nlohmann::json _data = nlohmann::json::parse(http->response->data);
			std::string token = _data["data"]["token"].get<std::string>();
			int jwt_status = tegia::threads::current()->context->jwt.unpack(token, this->jwt_public_key);

			message->data["responce"]["location"] = "/";
			message->data["responce"]["type"] = "302";
		}
		break;

		case 404:
		{
			(*responcedata)["status"] = "error";
			(*responcedata)["error"]["code"] = 404;
		}
		break;

		default:
		{
			(*responcedata)["status"] = "error";
			(*responcedata)["error"]["code"] = status;
		}
		break;
	}
	delete http;

	/////////////////////////////////////////////////////////////////////////////////////
	return "ok";
};


