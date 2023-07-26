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
				bot.global_command_create(dpp::slashcommand("give", "give money", bot.me.id).add_option(dpp::command_option(dpp::co_integer, "balance", "add more money", true)));
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