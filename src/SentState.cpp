#include <stdexcept>
#include "SentState.h"

void SentState::send(NotificationContext& notification)
{
	throw std::logic_error("Notification already sent");
}

void SentState::markDelivered(NotificationContext&)
{

}

void SentState::markDeliveryError(NotificationContext&)
{

}

void SentState::retry(NotificationContext&)
{
	throw std::logic_error("Notification must have a delivery error first");
}

std::string SentState::getName() const
{
	return "Sent";
}
