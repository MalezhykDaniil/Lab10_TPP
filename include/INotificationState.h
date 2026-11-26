#pragma once
#include <string>

class NotificationContext;

class INotificationState 
{
public:
	virtual ~INotificationState() = default;

	virtual void send(NotificationContext& context) = 0;

	virtual void markDelivered(NotificationContext& context) = 0;

	virtual void markDeliveryError(NotificationContext& context) = 0;

	virtual void retry(NotificationContext& context) = 0;

	virtual std::string getName() const = 0;
};