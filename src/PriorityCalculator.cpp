#include "PriorityCalculator.h"

void PriorityCalculator::setStrategy(std::unique_ptr<IPriorityStrategy> strategy)
{
	if (strategy == nullptr)
	{
		throw std::invalid_argument("Strategy can not be null");
	}

	strategy_ = std::move(strategy);

}

double PriorityCalculator::calculate(const Notification& notification) const
{
	if (strategy_ == nullptr)
	{
		throw std::logic_error("Strategy can not be null");
	}

	return strategy_->calculate(notification);
}