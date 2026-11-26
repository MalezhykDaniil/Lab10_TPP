#pragma once

#include <memory>
#include "IPriorityStrategy.h"
#include "IPriorityStrategyFactory.h"

class NormalizedWeightedSumPriorityFactory : public IPriorityStrategyFactory
{
public:
	std::unique_ptr<IPriorityStrategy> createStrategy() override;
};