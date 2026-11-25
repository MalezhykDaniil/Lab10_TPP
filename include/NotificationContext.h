#pragma once
#include <memory>
#include <utility>
#include <iostream>
#include "INotificationState.h"
#include "Notification.h"

class NotificationContext
{
private:
	std::unique_ptr<INotificationState> _state;
	Notification notification_;

public:
	explicit NotificationContext(std::unique_ptr<INotificationState> initialState, Notification notification)
		: _state(std::move(initialState)), notification_(std::move(notification)) {
	}

	void setState(std::unique_ptr<INotificationState> newState)
	{
		_state = std::move(newState);
	}

	void send()
	{
		_state->send(*this);
	}

	void markDelivered()
	{
		_state->markDelivered(*this);
	}

	void markDeliveryError()
	{
		_state->markDeliveryError(*this);
	}

	void retry()
	{
		_state->retry(*this);
	}

	void showState() const
	{
		std::cout << _state->getName() << "\n";
	}

	Notification& getNotification();
};