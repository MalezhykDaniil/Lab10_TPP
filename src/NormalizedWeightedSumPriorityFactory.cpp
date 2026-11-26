#include "NormalizedWeightedSumPriorityFactory.h"
#include "NormalizedWeightedSumPriorityStrategy.h"

std::unique_ptr<IPriorityStrategy> NormalizedWeightedSumPriorityFactory::createStrategy()
{
	return std::make_unique<NormalizedWeightedSumPriorityStrategy>();
}