#pragma once
#include "INotificationState.h"

class CreatedState : public INotificationState
{
public:
	void send(NotificationContext& notification) override;

	void markDelivered(NotificationContext&) override;

	void markDeliveryError(NotificationContext&) override;

	void retry(NotificationContext&) override;

	std::string getName() const override;
};