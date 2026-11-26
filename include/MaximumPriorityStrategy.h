#pragma once

#include "IPriorityStrategy.h"

class MaximumPriorityStrategy : public IPriorityStrategy 
{
public:
	double calculate(const Notification& notification) const override;
};