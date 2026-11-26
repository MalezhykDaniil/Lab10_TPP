#pragma once
#include <memory>
#include <utility>
#include <iostream>
#include <stdexcept>
#include "INotificationState.h"
#include "Notification.h"
#include "INotificationStateRepository.h"
#include "NotificationSubject.h"

class NotificationContext
{
private:
	std::unique_ptr<INotificationState> _state;
	Notification notification_;
	INotificationStateRepository& repository_;
	NotificationSubject* subject_ = nullptr;

public:
	explicit NotificationContext(std::unique_ptr<INotificationState> initialState, Notification notification, INotificationStateRepository& repository, NotificationSubject* subject = nullptr)
		: _state(std::move(initialState)), notification_(std::move(notification)), repository_(repository), subject_(subject)
	{
		if (_state == nullptr) {
			throw std::invalid_argument("State can not be null");
		}
	}

	void setState(std::unique_ptr<INotificationState> newState)
	{
		if (newState == nullptr) {
			throw std::invalid_argument("State can not be null");
		}

		std::string oldState = _state->getName();
		std::string newStateName = newState->getName();

		repository_.saveTransition(
			notification_.id,
			oldState,
			newStateName
		);

		_state = std::move(newState);

		if (subject_ != nullptr && newStateName == "Delivered")
		{
			subject_->notify({ notification_.id, newStateName });
		}
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

	std::string getStateName() const
	{
		return _state->getName();
	}
};