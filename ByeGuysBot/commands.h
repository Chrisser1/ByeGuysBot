#pragma once
#include <dpp/dpp.h>
#include <random>
#include <chrono>
#include "User.h"

class Commands
{
private:
    std::unordered_map<dpp::snowflake, User>& users;
    dpp::cluster& bot;

public:
    Commands(std::unordered_map<dpp::snowflake, User>& users, dpp::cluster& bot) :
        users(users), bot(bot) {}

    void run();

private:
    // User-related methods
    User& GetUser(const dpp::snowflake& userId) { return users[userId]; }
    bool IsRegistered(const dpp::slashcommand_t& event);
    void AddUser(const dpp::snowflake& userId, const User& user) { users[userId] = user; }
    void RegisterUser(const dpp::slashcommand_t& event, dpp::snowflake userId);
    void Give(const dpp::slashcommand_t& event);
    void Daily(const dpp::slashcommand_t& event);

    // Game-related methods
    void Gamble(const dpp::slashcommand_t& event);
    void HigherLower(const dpp::select_click_t& event);
    void ButtonPressHigherOrLower(const dpp::button_click_t& event);

    // Utility methods
    const void ShowBalance(const dpp::interaction_create_t& event);
    dpp::snowflake GetUserId(const dpp::interaction_create_t& event) { return event.command.get_issuing_user().id; }

    // Random number generation
    static double RandomDouble() {
        // Using high resolution clock to seed the random engine
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        return distribution(generator);
    }

    static double RandomDouble(double min, double max) {
        // Using high resolution clock to seed the random engine
        unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }

    // Random integer generation
    static int RandomInt(int min, int max) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> distribution(min, max);
        return distribution(generator);
    }
};