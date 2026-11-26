#include "DeliveryStatistics.h"

void DeliveryStatistics::update(const NotificationEvent& event)
{
    if (event.newState == "Delivered")
    {
        deliveredCount_++;
    }
}

int DeliveryStatistics::getDeliveredCount() const
{
	return deliveredCount_;
}