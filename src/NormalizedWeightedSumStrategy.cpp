#include <algorithm>

#include "NormalizedWeightedSumPriorityStrategy.h"

double NormalizedWeightedSumPriorityStrategy::calculate(const Notification& notification) const
{
    double waitingPriority = std::min(notification.waitingTime / 60.0 * 10.0, 10.0);

    return (notification.importance * 5 + notification.urgency * 3 + waitingPriority * 2)/(5+3+2);
}