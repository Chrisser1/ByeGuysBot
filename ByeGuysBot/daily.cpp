#include "commands.h"

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