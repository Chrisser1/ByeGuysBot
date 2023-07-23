#pragma once
#include "dpp/dpp.h"

struct GameData
{
	double currentNumber = NULL;
};

struct User
{
	dpp::snowflake UserID = NULL;
	std::string UserName = "Unknown";
	double LastDaily = 0.0;
	int Balance = 0;
	GameData gameData;
};