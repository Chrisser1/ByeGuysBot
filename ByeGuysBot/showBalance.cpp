#include "commands.h"

const void Commands::ShowBalance(const dpp::interaction_create_t& event)
{
	// Cast event to slashcommand_t
	const dpp::slashcommand_t& slashEvent = reinterpret_cast<const dpp::slashcommand_t&>(event);

	// get user
	User& user = GetUser(GetUserId(slashEvent));

	// Show users current balance
	int balance = user.balance;

	// Convert balance to string
	std::string balance_str = std::to_string(balance);

	// Append "DKK" to balance_str
	balance_str += " Bitch Coin(s)";

	dpp::embed embed = dpp::embed().
		set_color(dpp::colors::red_blood).
		set_title("!!" + user.userName + " balance!!").
		set_description(balance_str).
		set_timestamp(time(0));

	bot.message_create(dpp::message(slashEvent.command.channel_id, embed).set_reference(slashEvent.command.id));

	if (balance >= 10000)
		slashEvent.reply("oki richy boi");
	else if (balance <= 10)
		slashEvent.reply("this isn't free you know!");
	else
		slashEvent.reply("Okay! okay! I got it!");
}