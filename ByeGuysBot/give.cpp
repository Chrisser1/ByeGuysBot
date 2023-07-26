#include "commands.h"

void Commands::Give(const dpp::slashcommand_t& event)
{
	dpp::snowflake Chrisser = dpp::snowflake(310496936090402818);
	dpp::snowflake LukV = dpp::snowflake(365889587794673664);

	if (event.command.get_issuing_user().id != Chrisser && event.command.get_issuing_user().id != LukV) {
		dpp::embed sussy = dpp::embed().
		set_image("https://preview.redd.it/r7cublormi561.jpg?auto=webp&s=93173ab0e384d6e72ec87b78ec42cdab4c618d69");
		event.reply(dpp::message(event.command.channel_id, sussy).set_reference(event.command.id));
		return;
	}

	try {
		auto balanceParameter = event.get_parameter("balance");
		if (std::holds_alternative<std::int64_t>(balanceParameter)) {
			int balanceToAdd = std::get<std::int64_t>(balanceParameter);
			users[GetUserId(event)].balance += balanceToAdd;
		}
		else {
			// Handle the case where the balance parameter is not an integer.
			// You might want to log an error, set a default value, or take appropriate action.
			// For example:
			std::cout << "Error: 'balance' parameter is not of type int." << std::endl;
		}
	}
	catch (const std::invalid_argument& e) {
		event.reply("Invalid balance input.");
		return;
	}
	catch (const std::out_of_range& e) {
		event.reply("Balance input is out of range.");
		return;
	}
	dpp::embed embed = dpp::embed().
		set_color(dpp::colors::alien_green).
		set_title("oki ricky boi").
		add_field("New balance", std::to_string(users[GetUserId(event)].balance) + " Bitch Coin(s)");
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
}