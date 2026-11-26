#pragma once

#include <iostream>
#include "INotificationObserver.h"

class NotificationLogger : public INotificationObserver
{
public:
    void update(const NotificationEvent& event) override;
};