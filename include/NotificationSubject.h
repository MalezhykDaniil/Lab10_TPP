#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "INotificationObserver.h"

class NotificationSubject
{
private:
	std::vector<std::weak_ptr<INotificationObserver>> observers_;

public:
	void subscribe(const std::shared_ptr<INotificationObserver>& observer);
	
	void unsubscribe(const std::shared_ptr<INotificationObserver>& observer);
	
	void notify(const NotificationEvent& event);
};