#include <dpp/dpp.h>
#include <iostream>
#include <unordered_map>
#include "user.h"
#include "commands.h"
#include "dotenv.h"

inline double RandomDouble();



int main() {
	dotenv::load(".env");
	char* TOKEN;
	size_t envValueSize = 73;
	_dupenv_s(&TOKEN, &envValueSize, "TOKEN");
	dpp::cluster bot(TOKEN);
	free(TOKEN);

	std::unordered_map<dpp::snowflake, User> listOfUsers;
	Commands commands(listOfUsers);

	bot.on_log(dpp::utility::cout_logger());

	bot.on_slashcommand([&](const dpp::slashcommand_t& event) {

		if (commands.IsRegistered(event))
			return;

		if (event.command.get_command_name() == "ping") {
			event.reply("Pong!");
		}

		if (event.command.get_command_name() == "register") {
			commands.RegisterUser(event, event.command.get_issuing_user().id);
		}

		if (event.command.get_command_name() == "daily") {
			double number = RandomDouble();
			std::string nuberString = std::to_string(number);
			event.reply(nuberString);
		}

		if (event.command.get_command_name() == "gamble") {
			commands.Gamble(event);
		}
		});

	bot.on_ready([&bot](const dpp::ready_t& event)
		{
			if (dpp::run_once<struct register_bot_commands>()) {
				bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
				bot.global_command_create(dpp::slashcommand("register", "Register as a user of the bot", bot.me.id));
				bot.global_command_create(dpp::slashcommand("daily", "Get a random amount of money each day", bot.me.id));
				bot.global_command_create(dpp::slashcommand("gamble", "Gamble your money away", bot.me.id));
			}
		});

	bot.start(dpp::st_wait);


	return 0;
}

inline double RandomDouble()
{
	return rand() / (RAND_MAX + 1.0f);
}

