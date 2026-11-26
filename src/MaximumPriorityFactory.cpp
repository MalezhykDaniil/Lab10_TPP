#include "MaximumPriorityFactory.h"
#include "MaximumPriorityStrategy.h"

std::unique_ptr<IPriorityStrategy> MaximumPriorityFactory::createStrategy()
{
	return std::make_unique<MaximumPriorityStrategy>();
}