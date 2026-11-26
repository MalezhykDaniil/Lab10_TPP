#pragma once

#include <memory>
#include "IPriorityStrategy.h"
#include "IPriorityStrategyFactory.h"

class MaximumPriorityFactory : public IPriorityStrategyFactory
{
public:
	std::unique_ptr<IPriorityStrategy> createStrategy() override;
};