#include <stdexcept>
#include "NotificationStateRepository.h"
#include "Notification.h"

using namespace sqlite_orm;

void NotificationStateRepository::saveTransition(int notificationId, const std::string& oldState, const std::string& newState) 
{
    storage_.transaction([&]()
    {

        auto notification = storage_.get<Notification>(notificationId);
        if (notification.state != oldState) {
            throw std::logic_error("Notification state mismatch");
        }

        storage_.update_all(set(c(&Notification::state) = newState),
            where(c(&Notification::id) == notificationId)
        );

        NotificationStateHistory history{
            0,
            notificationId,
            oldState,
            newState
        };

        storage_.insert(history);

        return true;
    });
}
