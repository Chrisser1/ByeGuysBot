#include <dpp/dpp.h>
#include <iostream>
#include <unordered_map>
#include "user.h"
#include "commands.h"
#include "dotenv.h"

int main() {
    dotenv::load(".env");
    char* BOT_TOKEN;
    size_t envValueSize = 73;
    _dupenv_s(&BOT_TOKEN, &envValueSize, "TOKEN");
    dpp::cluster discordBot(BOT_TOKEN);
    free(BOT_TOKEN);

    std::unordered_map<dpp::snowflake, User> userMap;
    Commands botCommands(userMap, discordBot);

    botCommands.run();

    return 0;
}