#include <stdexcept>
#include "CreatedState.h"

void CreatedState::send(NotificationContext& notification)
{

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
