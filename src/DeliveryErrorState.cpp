#include <stdexcept>
#include <memory>
#include "SentState.h"
#include "NotificationContext.h"
#include "DeliveryErrorState.h"

void DeliveryErrorState::send(NotificationContext&)
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

void DeliveryErrorState::retry(NotificationContext& context)
{
	context.setState(std::make_unique<SentState>());
}

std::string DeliveryErrorState::getName() const
{
	return "DeliveryError";
}
