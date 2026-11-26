#pragma once

#include "IPriorityStrategy.h"

class NormalizedWeightedSumPriorityStrategy : public IPriorityStrategy
{
public:
	double calculate(const Notification& notification) const override;
};