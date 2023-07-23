#pragma once
#include <dpp/dpp.h>
#include "user.h"

class Commands
{
public:
    Commands(std::unordered_map<dpp::snowflake, User>& users) : listOfUsers(users) {}

    User& GetUser(const dpp::snowflake& userId) {
        return listOfUsers[userId];
    }

    bool IsRegistered(const dpp::slashcommand_t& event);

    void AddUser(const dpp::snowflake& userId, const User& user) {
        listOfUsers[userId] = user;
    }

    void RegisterUser(const dpp::slashcommand_t& event, dpp::snowflake userId);
    void Gamble(const dpp::slashcommand_t& event);

private:
    std::unordered_map<dpp::snowflake, User>& listOfUsers;
};
