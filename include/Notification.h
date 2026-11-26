#pragma once
#include <string>

struct Notification
{
    int id = 0;
    int categoryId = 0;
    std::string title;
    std::string message;
    std::string createdAt;
    int priority = 3;
    std::string state = "Created";
    int importance = 1;
    int urgency = 1;
    double waitingTime = 0.0;
};
