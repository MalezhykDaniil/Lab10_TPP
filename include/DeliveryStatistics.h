#pragma once

#include "INotificationObserver.h"

class DeliveryStatistics : public INotificationObserver
{
private:
    int deliveredCount_ = 0;

public:
    void update(const NotificationEvent& event) override;

    int getDeliveredCount() const;
};