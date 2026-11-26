#include <stdexcept>
#include <memory>
#include "DeliveredState.h"
#include "DeliveryErrorState.h"
#include "NotificationContext.h"
#include "SentState.h"

void SentState::send(NotificationContext&)
{
	throw std::logic_error("Notification already sent");
}

void SentState::markDelivered(NotificationContext& context)
{
	context.setState(std::make_unique<DeliveredState>());
}

void SentState::markDeliveryError(NotificationContext& context)
{
	context.setState(std::make_unique<DeliveryErrorState>());
}

void SentState::retry(NotificationContext&)
{
	throw std::logic_error("Notification must have a delivery error first");
}

std::string SentState::getName() const
{
	return "Sent";
}
