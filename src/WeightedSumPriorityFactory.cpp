#include "WeightedSumPriorityFactory.h"
#include "WeightedSumPriorityStrategy.h"

std::unique_ptr<IPriorityStrategy> WeightedSumPriorityFactory::createStrategy()
{
	return std::make_unique<WeightedSumPriorityStrategy>();
}