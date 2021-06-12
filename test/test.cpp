
#include<freeini.hpp>

#include<string>
#include<fstream>

bool make_default_config(std::string filename)
{
	std::fstream file(filename, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

	if(file.is_open())
	{
		file << "[DEFAULT]\nmy_option=my_value\nmy_second_option=http://127.0.0.1\n";
		return true;
	}

	file.close();

	return false;
}


int main(int argc, char* argv[])
{
	std::string configFilename("config.ini");

	int failCount = 0;

	if(make_default_config(configFilename))
	{
		IniParser parser;

		if(!parser.load(configFilename))
		{
			std::cout << "Load with filename failed!" << std::endl;
			failCount++;
			return failCount;
		}

		if(parser["DEFAULT"]["my_option"] != "my_value")
		{
			std::cout << "my_option != my_value | " << parser["DEFAULT"]["my_option"] << std::endl;
			failCount++;
		}

		if(parser["DEFAULT"]["my_second_option"] != "http://127.0.0.1")
		{
			std::cout << "my_second_option != http://127.0.0.1 | " << parser["DEFAULT"]["my_second_option"] << std::endl;
			failCount++;
		}

		parser["DEFAULT"]["my_new_option"] = "my_new_value";

		if(!parser.save(configFilename))
		{
			std::cout << "Save with filename failed!" << std::endl;
			failCount++;
			return failCount;
		}

		parser.clear();

		if(!parser.load(configFilename))
		{
			std::cout << "Load with filename failed!" << std::endl;
			failCount++;
			return failCount;
		}

		if(parser["DEFAULT"]["my_new_option"] != "my_new_value")
		{
			std::cout << "my_new_option != my_new_value | " << parser["DEFAULT"]["my_new_option"] << std::endl;
			failCount++;
		}
	}
	else
		return -1;

	return failCount;
}

