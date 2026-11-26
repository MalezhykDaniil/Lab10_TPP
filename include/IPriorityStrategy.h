#pragma once

#include "Notification.h"

class IPriorityStrategy 
{
public:
	virtual ~IPriorityStrategy() = default;

	virtual double calculate(const Notification& notification) const = 0;
};