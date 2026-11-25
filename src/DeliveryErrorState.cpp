#include <stdexcept>
#include "DeliveryErrorState.h"

void DeliveryErrorState::send(NotificationContext& notification)
{
	throw std::logic_error("Use retry() to resend notification");
}

void DeliveryErrorState::markDelivered(NotificationContext&)
{
	throw std::logic_error("Notification cannot be marked delivered in error state");
}

void DeliveryErrorState::markDeliveryError(NotificationContext&)
{
	throw std::logic_error("Notification already has delivery error");
}

void DeliveryErrorState::retry(NotificationContext&)
{

}

std::string DeliveryErrorState::getName() const
{
	return "DeliveryError";
}
