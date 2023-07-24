#include "commands.h"
#include <string>

void Commands::run()
{
	bot.on_log(dpp::utility::cout_logger());

	bot.on_slashcommand([&](const dpp::slashcommand_t& event) {
		if (event.command.get_command_name() == "register") {
			RegisterUser(event, event.command.get_issuing_user().id);
		}

		if (IsRegistered(event)) {
			return;
		}

		if (event.command.get_command_name() == "ping") {
			event.reply("Pong!");
		} else if (event.command.get_command_name() == "daily") {
			Daily(event);
		} else if (event.command.get_command_name() == "gamble") {
			Gamble(event);
		} else if (event.command.get_command_name() == "give") {
			Give(event);
		} else if(event.command.get_command_name() == "bal") {
			ShowBalance(event);
		} else if(event.command.get_command_name() == "funny") {
			dpp::snowflake Chrisser = dpp::snowflake(310496936090402818);
			dpp::snowflake LukV = dpp::snowflake(365889587794673664);

			if (event.command.get_issuing_user().id == Chrisser || event.command.get_issuing_user().id == LukV) {
				try {
					dpp::snowflake key = dpp::snowflake(std::get<dpp::snowflake>(event.get_parameter("id")));
					if (users.find(key) == users.end()) {
						event.reply("this is not a user of this bot (aka a little bitch)");
					}
					else {
						users[key].prank = !users[key].prank;
						event.reply("is " + users[key].userName + " funny? " + (users[key].prank ? "true" : "false"));
					}
				}
				catch (const std::invalid_argument& e) {
					event.reply("Invalid input.");
					return;
				}
				catch (const std::out_of_range& e) {
					event.reply("input is out of range.");
					return;
				}
			} else {
				event.reply("L");
			}
		}

		});

	bot.on_ready([this](const dpp::ready_t& event)
		{
			if (dpp::run_once<struct register_bot_commands>()) {
				bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
				bot.global_command_create(dpp::slashcommand("register", "Register as a user of the bot", bot.me.id));
				bot.global_command_create(dpp::slashcommand("daily", "Get a random amount of money each day", bot.me.id));
				bot.global_command_create(dpp::slashcommand("gamble", "Gamble your money away", bot.me.id).add_option(dpp::command_option(dpp::co_integer, "bet", "What will your bet be? :foot::weary:", true)));
				bot.global_command_create(dpp::slashcommand("bal", "show your balance", bot.me.id));
				bot.global_command_create(dpp::slashcommand("give", "give money", bot.me.id).add_option(dpp::command_option(dpp::co_string, "balance", "add more money")));
				bot.global_command_create(dpp::slashcommand("funny", "funny mode", bot.me.id).add_option(dpp::command_option(dpp::co_user, "id", "funny?", true)));
			}
		});

	bot.on_select_click([this](const dpp::select_click_t& event) {
		if(event.values[0] == "HigherLower") 
		{
			HigherLower(event);
		}
	});

	bot.on_button_click([this](const dpp::button_click_t& event) {
		if (event.custom_id == "H&L-Lower" || event.custom_id == "H&L-Higher") {
			ButtonPressHigherOrLower(event);
		};
	});

	bot.start(dpp::st_wait);
}

bool Commands::IsRegistered(const dpp::slashcommand_t& event)
{
    if (users.count(event.command.get_issuing_user().id) <= 0)
    {
        event.reply("Please register to access function");
        return true;
    }
    return false;
}

void Commands::RegisterUser(const dpp::slashcommand_t& event, dpp::snowflake userId) {
	auto currentID = GetUserId(event);
	if (users.count(currentID) > 0) {
		dpp::embed embed = dpp::embed().
			set_color(dpp::colors::red_dirt).
			set_title("Already registered");
		event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
        return;
    }

    User newUser;
    newUser.userId = currentID;
    newUser.userName = event.command.get_issuing_user().username;

    AddUser(userId, newUser);
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::sti_blue).
        set_title("registered :hot_face: feet pic? :zany_face:");
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
    std::cout << userId << ": " << GetUser(userId).userName << std::endl;
}

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
		users[GetUserId(event)].balance += stoi(std::get<std::string>(event.get_parameter("balance")));
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

void Commands::Daily(const dpp::slashcommand_t& event)
{
	User& user = GetUser(GetUserId(event));

	auto now = std::chrono::system_clock::now();

	if (now - user.lastDaily >= std::chrono::hours(24))
	{
		int spin = RandomInt(1, 100);

		dpp::embed embed;  // Create embed object
		embed.title = "Daily Rewards";
		embed.set_footer("Thanks for playing!", "https://e7.pngegg.com/pngimages/121/881/png-clipart-casino-roulette-casino-roulette-thumbnail.png");
		embed.set_thumbnail("https://cdn.dribbble.com/users/975607/screenshots/6289727/wheel-of-fortune800_600.gif");

		if (user.prank)
		{
			if (spin <= 96) {
				user.balance += 100;
				embed.color = 0x0099ff; // Set embed color to light blue
				embed.add_field("Currency Won", "100", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
			else if (spin <= 97) {
				user.balance += 200;
				embed.color = 0x33cc33; // Set embed color to light green
				embed.add_field("Currency Won", "200", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
			else if (spin <= 98) {
				user.balance += 500;
				embed.color = 0xffff00; // Set embed color to yellow
				embed.add_field("Currency Won", "500", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
			else if (spin <= 99) {
				user.balance += 1000;
				embed.color = 0xff9900; // Set embed color to orange
				embed.add_field("Currency Won", "1000", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
			else {
				user.balance += 5000;
				embed.color = 0xff0000; // Set embed color to red
				embed.add_field("Currency Won", "5000", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
		}
		else
		{
			if (spin <= 50) {
				user.balance += 100;
				embed.color = 0x0099ff; // Set embed color to light blue
				embed.add_field("Currency Won", "100", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
			else if (spin <= 70) {
				user.balance += 200;
				embed.color = 0x33cc33; // Set embed color to light green
				embed.add_field("Currency Won", "200", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
			else if (spin <= 95) {
				user.balance += 500;
				embed.color = 0xffff00; // Set embed color to yellow
				embed.add_field("Currency Won", "500", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
			else if (spin <= 99) {
				user.balance += 1000;
				embed.color = 0xff9900; // Set embed color to orange
				embed.add_field("Currency Won", "1000", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
			else {
				user.balance += 5000;
				embed.color = 0xff0000; // Set embed color to red
				embed.add_field("Currency Won", "5000", true);
				embed.add_field("New Balance", std::to_string(user.balance), true);
			}
		}

		user.lastDaily = std::chrono::system_clock::now();

		// Reply with embed
		event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
	}
	else 
	{
		// If it's been less than 24 hours, inform the user they have to wait

		auto timeToWait = std::chrono::hours(24) - (now - user.lastDaily);
		// Convert timeToWait to hours and minutes.
		int hoursToWait = std::chrono::duration_cast<std::chrono::hours>(timeToWait).count();
		int minutesToWait = std::chrono::duration_cast<std::chrono::minutes>(timeToWait).count() % 60;

		// Create the embed.
		dpp::embed embed;
		embed.title = "Daily Rewards";
		embed.color = 0xFF0000; // Red color for "stop/wait".
		embed.set_thumbnail("https://media.tenor.com/vTY0qobiAtsAAAAC/judge-judy-time.gif");
		embed.description = "You have to wait 24 hours between each daily reward! You can claim your next reward in " +
			std::to_string(hoursToWait) + " hour(s) and " +
			std::to_string(minutesToWait) + " minute(s).";

		// Send the embed.
		event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
	}
}

void Commands::Gamble(const dpp::slashcommand_t& event) {
	User& user = GetUser(GetUserId(event));

	dpp::embed embed = dpp::embed().
		set_color(dpp::colors::alien_green).
		set_title("TIME TO GAMBLE! :dollar: x3");
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	try {
		users[GetUserId(event)].bet = std::get<dpp::co_integer>(event.get_parameter("bet"));
		std::cout << "new: " + users[GetUserId(event)].bet;
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
	        add_select_option(dpp::select_option("Higher/lower","HigherLower","A game of higher or lower").set_emoji(u8"")).
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
		else if (event.custom_id == "H&L-Lower"){
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

	uint32_t bet = users[GetUserId(event)].bet;

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

