#include "NotificationSubject.h"

void NotificationSubject::subscribe(const std::shared_ptr<INotificationObserver>& observer)
{
	observers_.push_back(observer);
}

void NotificationSubject::unsubscribe(const std::shared_ptr<INotificationObserver>& observer)
{
    std::erase_if(observers_,
        [&observer](const std::weak_ptr<INotificationObserver>& item)
        {auto ptr = item.lock(); 
            return !ptr || ptr == observer;
        });
}

void NotificationSubject::notify(const NotificationEvent& event)
{
    std::erase_if(observers_,
        [](const std::weak_ptr<INotificationObserver>& item)
        {
            return item.expired();
        });

    for (const auto& item : observers_)
    {
        if (auto observer = item.lock())
        {
            observer->update(event);
        }
    }
}