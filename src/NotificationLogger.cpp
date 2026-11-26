#include "NotificationLogger.h"

void NotificationLogger::update(const NotificationEvent& event)
{
    std::cout << "Notification "
        << event.notificationId
        << " changed state to "
        << event.newState << '\n';
}