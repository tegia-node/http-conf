////////////////////////////////////////////////////////////////////////////////////////////
/**

	Инициализация http-сессии


	count1 = 890
	count2 = 9999110

*/
////////////////////////////////////////////////////////////////////////////////////////////


long long int HashFunctionHorner(const std::string& s, long long int table_size, const long long int key)
{
    long long int hash_result = 0;
    for (long long int i = 0; s[i] != s.size(); ++i)
        hash_result = (key * hash_result + s[i]) % table_size;
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

std::string A2Session::test(const std::shared_ptr<message_t> &message, const nlohmann::json &route_params)
{
	auto responcedata = &(message->data["responce"]["data"]);
	/////////////////////////////////////////////////////////////////////////////////////////  

	std::hash<std::string> hasher;

	//while(true)
	//{
		std::vector<std::string> table1(1000000);
		std::vector<std::string> table2(1000000);

		long long int key1 = 0;
		long long int key2 = 0;

		long long int count1 = 0;
		long long int count2 = 0;

		for(long long int i = 0; i < 1000000; i++)
		{
			std::string random = tegia::random::string(50);

			key1 = HashFunctionHorner(random,1000000,999999);
			key2 = HashFunctionHorner(random,1000000,1000001);

			std::cout << random << " " << hasher(random) << " " << key1 << " " << key2 << std::endl;

			if(table1[key1] == "")
			{
				table1[key1] = random;
				count1++;
			}
			else
			{
				std::cout << _ERR_TEXT_ << "first collision" << std::endl;
				std::cout << "key1 = " << key1 << std::endl;
				std::cout << "old  = " << table1[key1] << std::endl;
				std::cout << "new  = " << random << std::endl;

				if(table2[key2] == "")
				{
					table2[key2] == random;
					count2++;
				}
				else
				{
					std::cout << _ERR_TEXT_ << "second collision" << std::endl;
					std::cout << "key2 = " << key2 << std::endl;
					std::cout << "old  = " << table2[key2] << std::endl;
					std::cout << "new  = " << random << std::endl;
					exit(0);				
				}
			}
		}
	//}

	std::cout << "count1 = " << count1 << std::endl;
	std::cout << "count2 = " << count2 << std::endl;

	(*responcedata)["status"] = 0;
	(*responcedata)["data"]["name"] = this->name;
	(*responcedata)["data"]["hash"] = hasher(this->name);


	/////////////////////////////////////////////////////////////////////////////////////  
	return "ok";
};
