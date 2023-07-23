#include "commands.h"

void Commands::run()
{
	bot.on_log(dpp::utility::cout_logger());

	bot.on_slashcommand([&](const dpp::slashcommand_t& event) {
		if (event.command.get_command_name() == "register") {
			RegisterUser(event, event.command.get_issuing_user().id);
		}

		if (IsRegistered(event))
			return;

		if (event.command.get_command_name() == "ping") {
			event.reply("Pong!");
		}

		if (event.command.get_command_name() == "daily") {
			double number = RandomDouble();
			std::string nuberString = std::to_string(number);
			event.reply(nuberString);
		}

		if (event.command.get_command_name() == "gamble") {
			Gamble(event);
		}

		if(event.command.get_command_name() == "give") {
			listOfUsers[GetId(event)].Balance += stoi(std::get<std::string>(event.get_parameter("balance")));
			event.reply("oki richy boi");
		}
		
		if(event.command.get_command_name() == "bal") {
			ShowBalance(event);
		}

		});

	bot.on_ready([this](const dpp::ready_t& event)
		{
			if (dpp::run_once<struct register_bot_commands>()) {
				bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
				bot.global_command_create(dpp::slashcommand("register", "Register as a user of the bot", bot.me.id));
				bot.global_command_create(dpp::slashcommand("daily", "Get a random amount of money each day", bot.me.id));
				bot.global_command_create(dpp::slashcommand("gamble", "Gamble your money away", bot.me.id));
				bot.global_command_create(dpp::slashcommand("bal", "show your balance", bot.me.id));
				bot.global_command_create(dpp::slashcommand("give", "give money", bot.me.id).add_option(dpp::command_option(dpp::co_string, "balance", "add more money")));
			}
		});

	bot.on_select_click([this](const dpp::select_click_t& event) {
		if(event.custom_id == "GameID") {
			if(event.values[0] == "HigherLower") {
				HigherLower(event);
			}
		}
	});
	bot.on_button_click([this](const dpp::button_click_t& event) {
		/* Button clicks are still interactions, and must be replied to in some form to
		 * prevent the "this interaction has failed" message from Discord to the user.
		 */
		event.reply("You clicked: " + event.custom_id);
	});

	bot.start(dpp::st_wait);
}

bool Commands::IsRegistered(const dpp::slashcommand_t& event)
{
    if (listOfUsers.count(event.command.get_issuing_user().id) <= 0)
    {
        event.reply("Please register to access function");
        return true;
    }
    return false;
}

void Commands::RegisterUser(const dpp::slashcommand_t& event, dpp::snowflake userId) {
	auto currentID = GetId(event);
	if (listOfUsers.count(currentID) > 0) {
        event.reply("Already registered");
        return;
    }

    User newUser;
    newUser.UserID = currentID;
    newUser.UserName = event.command.get_issuing_user().username;

    AddUser(userId, newUser);
    dpp::embed embed = dpp::embed().
        set_color(dpp::colors::sti_blue).
        set_title("registered");
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
    std::cout << userId << ": " << GetUser(userId).UserName << std::endl;
}

void Commands::Gamble(const dpp::slashcommand_t& event) {
    auto thisID = event.command.get_issuing_user().id;
    if (GetUser(thisID).Balance <= 0) 
    {
		dpp::embed embed = dpp::embed().
			set_color(dpp::colors::sti_blue).
			set_title("Get:moneybag: L");
		event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
        return;
    }


	dpp::message m(event.command.channel_id, "Chose a game to gamble in");
	m.add_component(
	    dpp::component().add_component(
	        dpp::component().set_type(dpp::cot_selectmenu).
	        set_placeholder("Pick game").
	        add_select_option(dpp::select_option("Higher/lower","HigherLower","A game of higher or lower").set_emoji(u8"")).
	        add_select_option(dpp::select_option("label2","value2","description2").set_emoji(u8"")).
	        set_id("myselid")
	    )
	);
	event.reply(m);
}

void Commands::HigherLower(const dpp::select_click_t& event)
{
	User& thisUser = GetUser(GetId(event));

	ShowBalance(event);

	double min = 0.0, max = 100.0; // Choose appropriate min and max values
	double number = RandomDouble(min, max);
	
	// Set the users gameData.currentNumber to the random number
	thisUser.gameData.currentNumber = number;

	std::cout << thisUser.gameData.currentNumber;

	bot.message_create(
		dpp::message(event.command.channel_id, dpp::embed().
			set_color(dpp::colors::sti_blue).
			set_title("Get:moneybag: L"))
			//.add_component(
			
		/*dpp::component().add_component(
				dpp::component().set_label("Higher").
				set_type(dpp::cot_button).
				set_emoji(u8"").
				set_style(dpp::cos_danger).
				set_id("H&L-Higher")
			)
			.add_component(
				dpp::component().set_label("Lower").
				set_type(dpp::cot_button).
				set_emoji(u8"").
				set_style(dpp::cos_danger).
				set_id("H&L-Lower")
			)*/
		//)
	);

}

const void Commands::ShowBalance(const dpp::interaction_create_t& event)
{
	// Show users current balance
	int balance = GetUser(GetId(event)).Balance;

	// Convert balance to string
	std::string balance_str = std::to_string(balance);

	// Append "DKK" to balance_str
	balance_str += " Bitch Coin";

	dpp::embed embed = dpp::embed().
		set_color(dpp::colors::red_blood).
		set_title("!!YOUR BALANCE!!").
		set_description(balance_str).
		set_timestamp(time(0));

	bot.message_create(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	if (balance >= 10000)
		event.reply("oki richy boi");
	else if (balance <= 10)
		event.reply("this isn't free you know!");
	else
		event.reply("Okay! okay! I got it!");
}
