#pragma once

#include "NotificationEvent.h"

class INotificationObserver
{
public:
	virtual ~INotificationObserver() = default;
	
	virtual void update(const NotificationEvent& event) = 0;
};