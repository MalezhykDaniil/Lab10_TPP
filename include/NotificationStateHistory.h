#pragma once
#include <string>

struct NotificationStateHistory
{
	int id = 0;
	int notificationId = 0;
	std::string oldState;
	std::string newState;
};