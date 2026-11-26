#pragma once
#include <string>
#include <sqlite_orm/sqlite_orm.h>
#include "NotificationCategory.h"
#include "Notification.h"
#include "NotificationStateHistory.h"

inline auto createStorage(const std::string& filename)
{
    using namespace sqlite_orm;

    return make_storage(
        filename,
        
        make_table(
            "NotificationCategory",
            
            make_column("id", &NotificationCategory::id, primary_key().autoincrement()),
            
            make_column("name", &NotificationCategory::name, not_null()),
            
            make_column("description", &NotificationCategory::description, not_null()),
            
            make_column("channel", &NotificationCategory::channel, not_null()),
            
            make_column("defaultPriority", &NotificationCategory::defaultPriority, not_null())
        ),
        
        make_table(
            "Notification",
            
            make_column("id", &Notification::id, primary_key().autoincrement()),
            
            make_column("categoryId", &Notification::categoryId, not_null()),
            
            make_column("title", &Notification::title, not_null()),
            
            make_column("message", &Notification::message, not_null()),
            
            make_column("createdAt", &Notification::createdAt, not_null()),
            
            make_column("priority", &Notification::priority, not_null()),

            make_column("state", &Notification::state, not_null()),

            make_column("importance", &Notification::importance, not_null()),

            make_column("urgency", &Notification::urgency, not_null()),

            make_column("waitingTime", &Notification::waitingTime, not_null()),

            foreign_key(&Notification::categoryId)
                .references(&NotificationCategory::id)
        ),

        make_table(
            "NotificationStateHistory",

            make_column("id", &NotificationStateHistory::id, primary_key().autoincrement()),

            make_column("notificationId", &NotificationStateHistory::notificationId, not_null()),

            make_column("oldState", &NotificationStateHistory::oldState, not_null()),

            make_column("newState", &NotificationStateHistory::newState, not_null()),

            foreign_key(&NotificationStateHistory::notificationId)
                .references(&Notification::id)
        )
    );
}
