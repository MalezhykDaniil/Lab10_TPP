#pragma once
#include "INotificationState.h"

class SentState : public INotificationState
{
public:
	void send(NotificationContext&) override;

	void markDelivered(NotificationContext& notification) override;

	void markDeliveryError(NotificationContext& notification) override;

	void retry(NotificationContext&) override;

	std::string getName() const override;
};