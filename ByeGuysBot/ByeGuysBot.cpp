#include <dpp/dpp.h>
#include <iostream>
#include <unordered_map>
#include "user.h"
#include "commands.h"
#include "dotenv.h"

int main() {
	dotenv::load(".env");
	char* TOKEN;
	size_t envValueSize = 73;
	_dupenv_s(&TOKEN, &envValueSize, "TOKEN");
	dpp::cluster bot(TOKEN);
	free(TOKEN);

	std::unordered_map<dpp::snowflake, User> listOfUsers;
	Commands commands(listOfUsers, bot);
	
	commands.run();

	return 0;
}



