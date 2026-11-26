#pragma once

#include <memory>
#include "IPriorityStrategy.h"

class IPriorityStrategyFactory
{
public:
	virtual ~IPriorityStrategyFactory() = default;

	virtual std::unique_ptr<IPriorityStrategy> createStrategy() = 0;
};