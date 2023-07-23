#include "commands.h"

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
    if (listOfUsers.count(event.command.get_issuing_user().id) > 0) {
        event.reply("Already registered");
        return;
    }

    User newUser;
    newUser.UserID = userId;
    newUser.UserName = event.command.get_issuing_user().username;

    AddUser(userId, newUser);
    event.reply("registered");
    std::cout << userId << ": " << GetUser(userId).UserName << std::endl;
}

void Commands::Gamble(const dpp::slashcommand_t& event) {
    event.reply("W.I.P.");
}
