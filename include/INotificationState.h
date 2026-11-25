#pragma once
#include <string>

class NotificationContext;

class INotificationState 
{
public:
	virtual ~INotificationState() = default;

	virtual void send(NotificationContext& notification) = 0;

	virtual void markDelivered(NotificationContext& notification) = 0;

	virtual void markDeliveryError(NotificationContext& notification) = 0;

	virtual void retry(NotificationContext& notification) = 0;

	virtual std::string getName() const = 0;
};