

namespace HTTP {

int CONNECTION::response_200(const std::shared_ptr<message_t> &message)
{
	if("application/json" == message->data["response"]["content_type"].get<std::string>())
	{
		
		return 200;	
	}
};

}