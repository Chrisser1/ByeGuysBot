#include "commands.h"

void Commands::Gamble(const dpp::slashcommand_t& event) {
	User& user = GetUser(GetUserId(event));

	dpp::embed embed = dpp::embed().
		set_color(dpp::colors::alien_green).
		set_title("TIME TO GAMBLE! :dollar: x3");
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	try {
		auto betParameter = event.get_parameter("bet");
		if (std::holds_alternative<std::int64_t>(betParameter)) {
			int betToAdd = std::get<std::int64_t>(betParameter);
			users[GetUserId(event)].bet = betToAdd;
		}
		else {
			// Handle the case where the balance parameter is not an integer.
			// You might want to log an error, set a default value, or take appropriate action.
			// For example:
			std::cout << "Error: 'balance' parameter is not of type int." << std::endl;
		}
	}
	catch (const std::invalid_argument& e) {
		event.reply("Invalid bet amount input.");
		return;
	}
	catch (const std::out_of_range& e) {
		event.reply("Bet amount input is out of range.");
		return;
	}
	catch (const std::bad_variant_access& e) {
		event.reply("Incorrect type for bet amount input.");
		return;
	}



	if (user.balance < users[GetUserId(event)].bet)
	{
		dpp::embed embed = dpp::embed().
			set_color(dpp::colors::sti_blue).
			set_title("Get:moneybag: L");
		bot.message_create(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
		return;
	}

	dpp::message m(event.command.channel_id, "Chose a game to gamble in");
	m.add_component(
		dpp::component().add_component(
			dpp::component().set_type(dpp::cot_selectmenu).
			set_placeholder("Pick game").
			add_select_option(dpp::select_option("Higher/lower", "HigherLower", "A game of higher or lower").set_emoji(u8"")).
			set_id("GameId")
		)
	);

	bot.message_create(m, [&](const dpp::confirmation_callback_t& callback) {
		if (callback.is_error()) {
			std::cout << "Failed to send message: " << callback.get_error().message << "\n";
			return;
		}
		else {
			const auto& message = std::get<dpp::message>(callback.value);
			user.gameData.message.lastChannelId = message.channel_id;
			user.gameData.message.lastMessageId = message.id;
		}
		});
}

