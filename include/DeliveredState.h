#pragma once
#include "INotificationState.h"

class DeliveredState : public INotificationState
{
public:
	void send(NotificationContext&) override;

	void markDelivered(NotificationContext&) override;

	void markDeliveryError(NotificationContext&) override;

	void retry(NotificationContext&) override;

	std::string getName() const override;
};