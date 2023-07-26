#pragma once
#include "dpp/dpp.h"

struct Message
{
	dpp::snowflake lastMessageId = 0;
	dpp::snowflake lastChannelId = 0;
};

struct GameData
{
	double currentNumber = 0.0;
	Message message;
};

struct User
{
	dpp::snowflake userId = 0;
	std::string userName = "Unknown"; 
	std::chrono::system_clock::time_point lastDaily;
	int balance = 10000;
	GameData gameData;
	int bet = 0;
	bool prank = false;
};