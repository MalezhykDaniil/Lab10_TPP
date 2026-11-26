#include <algorithm>

#include "WeightedSumPriorityStrategy.h"

double WeightedSumPriorityStrategy::calculate(const Notification& notification) const
{
    double waitingPriority = std::min(notification.waitingTime / 60.0 * 10.0, 10.0);

    return notification.importance * 0.5 + notification.urgency * 0.3 + waitingPriority * 0.2;
}