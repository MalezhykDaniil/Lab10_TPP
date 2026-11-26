#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include "IPriorityStrategy.h"

class PriorityCalculator
{
private:
	std::unique_ptr<IPriorityStrategy> strategy_;

public:
	explicit PriorityCalculator(std::unique_ptr<IPriorityStrategy> strategy)
		: strategy_(std::move(strategy)) 
	{
		if (strategy_ == nullptr)
		{
			throw std::invalid_argument("Strategy can not be null");
		}

	}

	void setStrategy(std::unique_ptr<IPriorityStrategy> strategy);

	double calculate(const Notification& notification) const;
};