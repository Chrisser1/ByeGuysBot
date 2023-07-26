#include "commands.h"

void Commands::HigherLower(const dpp::select_click_t& event)
{
	User& user = GetUser(GetUserId(event));
	if (user.gameData.message.lastMessageId != 0 && user.gameData.message.lastChannelId != 0)
		bot.message_delete(user.gameData.message.lastMessageId, user.gameData.message.lastChannelId);

	event.reply();

	// Set the users gameData.currentNumber to the random number
	user.gameData.currentNumber = RandomDouble(0.0, 100.0);

	dpp::embed embed = dpp::embed().
		set_color(dpp::colors::sti_blue).
		set_title("Higher or Lower?").
		set_description("Is the unknown number higher or lower than: " + std::to_string(user.gameData.currentNumber));

	dpp::message m = dpp::message(event.command.channel_id, embed).add_component(
		dpp::component().add_component(
			dpp::component().set_label("Higher").
			set_type(dpp::cot_button).
			set_emoji(u8"").
			set_style(dpp::cos_danger).
			set_id("H&L-Higher")
		).add_component(
			dpp::component().set_label("Lower").
			set_type(dpp::cot_button).
			set_emoji(u8"").
			set_style(dpp::cos_danger).
			set_id("H&L-Lower")
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

void Commands::ButtonPressHigherOrLower(const dpp::button_click_t& event) {
	User& user = GetUser(GetUserId(event));
	if (user.gameData.message.lastMessageId != 0 && user.gameData.message.lastChannelId != 0)
		bot.message_delete(user.gameData.message.lastMessageId, user.gameData.message.lastChannelId);

	double randomNumber;
	if (user.prank)
	{
		bool failed;
		if (event.custom_id == "H&L-Higher") {
			randomNumber = RandomDouble(0.0, user.gameData.currentNumber);
		}
		else if (event.custom_id == "H&L-Lower") {
			randomNumber = RandomDouble(user.gameData.currentNumber, 100.0);
		}
	}
	else
		randomNumber = RandomDouble(0.0, 100.0);

	bool failed;
	if (event.custom_id == "H&L-Higher")
		failed = randomNumber < user.gameData.currentNumber;
	else if (event.custom_id == "H&L-Lower")
		failed = randomNumber > user.gameData.currentNumber;
	else {
		event.reply("Invalid button ID.");
		return;
	}

	int bet = users[GetUserId(event)].bet;

	if (failed)
		user.balance -= bet;
	else
		user.balance += bet;

	dpp::embed embed = dpp::embed().
		set_color(failed ? dpp::colors::red : dpp::colors::green).
		set_title(failed ? event.command.get_issuing_user().username + " gets only big Ls" : event.command.get_issuing_user().username + " gets only gets fat DUBZ").
		add_field("Your balance: " + std::to_string(user.balance) + " Bitch Coin(s)", "").
		add_field("The unknown number is: " + std::to_string(randomNumber), "");
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
}
