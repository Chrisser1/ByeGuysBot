#pragma once
#include <dpp/dpp.h>
#include "user.h"

inline double RandomDouble() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * RandomDouble();
}

class Commands
{
private:
    std::unordered_map<dpp::snowflake, User>& listOfUsers;
    dpp::cluster& bot;

public:
    Commands(std::unordered_map<dpp::snowflake, User>& users, dpp::cluster& bot) :
        listOfUsers(users), bot(bot) {}

    void run();

private:
    User& GetUser(const dpp::snowflake& userId) {
        return listOfUsers[userId];
    }

    dpp::snowflake GetId(const dpp::interaction_create_t& event) { return event.command.get_issuing_user().id; }

    bool IsRegistered(const dpp::slashcommand_t& event);

    void AddUser(const dpp::snowflake& userId, const User& user) {
        listOfUsers[userId] = user;
    }

    void RegisterUser(const dpp::slashcommand_t& event, dpp::snowflake userId);
    void Gamble(const dpp::slashcommand_t& event);
    
    void HigherLower(const dpp::select_click_t& event);

    const void ShowBalance(const dpp::interaction_create_t& event);
};
