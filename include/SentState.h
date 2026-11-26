#pragma once
#include "INotificationState.h"

class SentState : public INotificationState
{
public:
	void send(NotificationContext&) override;

	void markDelivered(NotificationContext& context) override;

	void markDeliveryError(NotificationContext& context) override;

	void retry(NotificationContext&) override;

	std::string getName() const override;
};