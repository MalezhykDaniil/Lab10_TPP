#pragma once

#include <string>
#include "Database.h"
#include "INotificationStateRepository.h"

using Storage = decltype(createStorage("database.sqlite"));

class NotificationStateRepository : public INotificationStateRepository
{
private:
	Storage& storage_;

public:
	explicit NotificationStateRepository(Storage& storage)
		: storage_(storage) { }

	void saveTransition(
		int notificationId,
		const std::string& oldState,
		const std::string& newState
	) override;
};