#include <stdexcept>
#include "DeliveredState.h"

void DeliveredState::send(NotificationContext& notification)
{
	throw std::logic_error("Notification already delivered");
}

void DeliveredState::markDelivered(NotificationContext&)
{
	throw std::logic_error("Notification already delivered");
}

void DeliveredState::markDeliveryError(NotificationContext&)
{
	throw std::logic_error("Notification already delivered");
}

void DeliveredState::retry(NotificationContext&)
{
	throw std::logic_error("Notification already delivered");
}

std::string DeliveredState::getName() const
{
	return "Delivered";
}
