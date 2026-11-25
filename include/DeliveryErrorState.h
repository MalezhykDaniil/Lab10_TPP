#pragma once
#include "INotificationState.h"

class DeliveryErrorState : public INotificationState
{
public:
	void send(NotificationContext&) override;

	void markDelivered(NotificationContext&) override;

	void markDeliveryError(NotificationContext&) override;

	void retry(NotificationContext& notification) override;

	std::string getName() const override;
};