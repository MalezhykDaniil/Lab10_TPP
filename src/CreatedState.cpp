#include <stdexcept>
#include <memory>
#include "CreatedState.h"
#include "NotificationContext.h"
#include "SentState.h"

void CreatedState::send(NotificationContext& context)
{
	context.setState(std::make_unique<SentState>());
}

void CreatedState::markDelivered(NotificationContext&)
{
	throw std::logic_error("Notification must be sent first");
}

void CreatedState::markDeliveryError(NotificationContext&)
{
	throw std::logic_error("Notification must be sent first");
}

void CreatedState::retry(NotificationContext&)
{
	throw std::logic_error("Notification must be sent first");
}

std::string CreatedState::getName() const
{
	return "Created";
}
