#include <algorithm>

#include "MaximumPriorityStrategy.h"

double MaximumPriorityStrategy::calculate(const Notification& notification) const
{
    double waitingPriority = std::min(notification.waitingTime / 60.0 * 10.0, 10.0);

    return std::max({ static_cast<double>(notification.importance), static_cast<double>(notification.urgency), waitingPriority });
}
