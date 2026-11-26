#pragma once

#include <string>

class INotificationStateRepository
{
public:
    virtual ~INotificationStateRepository() = default;

    virtual void saveTransition(
        int notificationId,
        const std::string& oldState,
        const std::string& newState
    ) = 0;
};