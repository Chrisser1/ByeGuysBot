#pragma once
#include "dpp/dpp.h"

struct User
{
	dpp::snowflake UserID = NULL;
	std::string UserName = "Unknown";
	double lastDaily = 0;
	double money = 0;
};