#pragma once

#include "IPriorityStrategy.h"

class WeightedSumPriorityStrategy : public IPriorityStrategy
{
public:
	double calculate(const Notification& notification) const override;
};