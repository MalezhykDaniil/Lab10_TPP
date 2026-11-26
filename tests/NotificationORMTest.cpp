#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>

#include "INotificationStateRepository.h"
#include "NotificationContext.h"
#include "CreatedState.h"
#include "SentState.h"
#include "DeliveredState.h"
#include "DeliveryErrorState.h"
#include "NotificationStateRepository.h"
#include "MaximumPriorityStrategy.h"
#include "WeightedSumPriorityStrategy.h"
#include "NormalizedWeightedSumPriorityStrategy.h"
#include "PriorityCalculator.h"
#include "NotificationSubject.h"
#include "NotificationLogger.h"
#include "DeliveryStatistics.h"
#include "MaximumPriorityFactory.h"
#include "WeightedSumPriorityFactory.h"
#include "NormalizedWeightedSumPriorityFactory.h"

class FakeNotificationStateRepository : public INotificationStateRepository
{
public:
    struct Transition
    {
        int notificationId;
        std::string oldState;
        std::string newState;
    };

    std::vector<Transition> transitions;
    bool shouldFail = false;

    void saveTransition(
        int notificationId,
        const std::string& oldState,
        const std::string& newState
    ) override
    {
        if (shouldFail) {
            throw std::runtime_error("Database error");
        }

        transitions.push_back({
            notificationId,
            oldState,
            newState
            });
    }
};

class TestObserver : public INotificationObserver
{
public:
    int updateCount = 0;
    NotificationEvent lastEvent{};

    void update(const NotificationEvent& event) override
    {
        updateCount++;
        lastEvent = event;
    }
};

TEST(NotificationContext, CreatesState)
{
    FakeNotificationStateRepository repository;
    
    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );
    
    EXPECT_EQ(context.getStateName(), "Created");
}

TEST(NotificationContext, CreatedToSent)
{
    FakeNotificationStateRepository repository;
    
    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    context.send();

    EXPECT_EQ(context.getStateName(), "Sent");
}

TEST(NotificationContext, SentToDelivered)
{
    FakeNotificationStateRepository repository;
    
    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    context.send();
    context.markDelivered();

    EXPECT_EQ(context.getStateName(), "Delivered");
}

TEST(NotificationContext, SentToDeliveryError)
{
    FakeNotificationStateRepository repository;

    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    context.send();
    context.markDeliveryError();

    EXPECT_EQ(context.getStateName(), "DeliveryError");
}

TEST(NotificationContext, DeliveryErrorToRetry)
{
    FakeNotificationStateRepository repository;
    
    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    context.send();
    context.markDeliveryError();
    context.retry();

    EXPECT_EQ(context.getStateName(), "Sent");
}

TEST(NotificationContext, CreatedCannotBeDelivered)
{
    FakeNotificationStateRepository repository;
    
    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    EXPECT_THROW(context.markDelivered(), std::logic_error);
}

TEST(NotificationContext, SentCannotBeSentAgain)
{
    FakeNotificationStateRepository repository;
    
    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    context.send();

    EXPECT_THROW(context.send(), std::logic_error);
}

TEST(NotificationContext, DeliveredIsTerminal)
{
    FakeNotificationStateRepository repository;
    
    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    context.send();
    context.markDelivered();

    EXPECT_THROW(context.send(), std::logic_error);
    EXPECT_THROW(context.markDelivered(), std::logic_error);
    EXPECT_THROW(context.markDeliveryError(), std::logic_error);
    EXPECT_THROW(context.retry(), std::logic_error);
}

TEST(NotificationState, CannotCreateContextWithoutState)
{
    FakeNotificationStateRepository repository;
    
    EXPECT_THROW(NotificationContext(nullptr, Notification{}, repository),
        std::invalid_argument);
}

TEST(NotificationState, CannotSetNullState)
{
    FakeNotificationStateRepository repository;
    
    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    EXPECT_THROW(context.setState(nullptr), std::invalid_argument);
}

TEST(NotificationStateRepository, RejectsIncorrectOldState)
{
    auto storage = createStorage(":memory:");
    NotificationStateRepository repository(storage);

    storage.on_open = [](sqlite3* db)
        {
            if (sqlite3_exec(
                db,
                "PRAGMA foreign_keys = ON;",
                nullptr,
                nullptr,
                nullptr
            ) != SQLITE_OK)
            {
                throw std::runtime_error(
                    "Не вдалося увімкнути зовнішні ключі"
                );
            }
        };

    storage.sync_schema();

    NotificationCategory category
    { 0, "Безпека транзакції", "Категорія, створена в транзакції", "email", 1
    };

    int categoryId = storage.insert(category);

    Notification notification
    { 0, categoryId, "Перевірка транзакції", "Категорію та сповіщення додано атомарно", "2026-11-11 14:10", 1 };

    int notificationId = storage.insert(notification);

    EXPECT_THROW(
        repository.saveTransition(notificationId, "Sent", "Delivered"),
        std::logic_error
    );

    auto savedNotification = storage.get<Notification>(notificationId);

    EXPECT_EQ(savedNotification.state, "Created");
    EXPECT_EQ(storage.count<NotificationStateHistory>(), 0);
}

TEST(NotificationStateRepository, RollsBackWhenHistoryInsertFails)
{
    auto storage = createStorage(":memory:");
    NotificationStateRepository repository(storage);

    storage.on_open = [](sqlite3* db)
        {
            if (sqlite3_exec(
                db,
                "PRAGMA foreign_keys = ON;",
                nullptr,
                nullptr,
                nullptr
            ) != SQLITE_OK)
            {
                throw std::runtime_error(
                    "Не вдалося увімкнути зовнішні ключі"
                );
            }
        };

    storage.sync_schema();

    NotificationCategory category
    {
        0,
        "Безпека транзакції",
        "Категорія для перевірки відкату",
        "email",
        1
    };

    int categoryId = storage.insert(category);

    Notification notification
    {
        0,
        categoryId,
        "Перевірка транзакції",
        "Перевірка атомарності операцій",
        "2026-11-11 14:10",
        1
    };

    int notificationId = storage.insert(notification);

    storage.drop_table("NotificationStateHistory");

    EXPECT_ANY_THROW(
        repository.saveTransition(notificationId, "Created", "Sent")
    );

    auto savedNotification = storage.get<Notification>(notificationId);

    EXPECT_EQ(savedNotification.state, "Created");
}

TEST(NotificationState, StateDoesNotChangeWhenRepositoryFails)
{
    FakeNotificationStateRepository repository;
    repository.shouldFail = true;

    NotificationContext context(
        std::make_unique<CreatedState>(),
        Notification{},
        repository
    );

    EXPECT_THROW(context.send(), std::runtime_error);

    EXPECT_EQ(context.getStateName(), "Created");
}

TEST(MaximumPriorityStrategy, MaximumPriority)
{
    Notification notification;
    notification.importance = 8;
    notification.urgency = 5;
    notification.waitingTime = 30;

    MaximumPriorityStrategy strategy;

    EXPECT_DOUBLE_EQ(strategy.calculate(notification), 8.0);
}

TEST(WeightedSumPriorityStrategy, WeightedSumPriority)
{
    Notification notification;
    notification.importance = 8;
    notification.urgency = 5;
    notification.waitingTime = 30;

    WeightedSumPriorityStrategy strategy;

    EXPECT_DOUBLE_EQ(strategy.calculate(notification), 6.5);
}

TEST(PriorityStrategy, NormalizedWeightedSumPriority)
{
    Notification notification;
    notification.importance = 8;
    notification.urgency = 5;
    notification.waitingTime = 30;

    NormalizedWeightedSumPriorityStrategy strategy;

    EXPECT_DOUBLE_EQ(strategy.calculate(notification), 6.5);
}

TEST(PriorityCalculator, SwitchesStrategy)
{
    Notification notification;
    notification.importance = 8;
    notification.urgency = 5;
    notification.waitingTime = 30;

    PriorityCalculator calculator(
        std::make_unique<MaximumPriorityStrategy>()
    );

    EXPECT_DOUBLE_EQ(calculator.calculate(notification), 8.0);

    calculator.setStrategy(
        std::make_unique<WeightedSumPriorityStrategy>()
    );

    EXPECT_DOUBLE_EQ(calculator.calculate(notification), 6.5);
}

TEST(PriorityCalculator, RejectsNullStrategyInConstructor)
{
    EXPECT_THROW(PriorityCalculator(nullptr), std::invalid_argument);
}

TEST(PriorityCalculator, RejectsNullStrategyInSetter)
{
    PriorityCalculator calculator(std::make_unique<MaximumPriorityStrategy>());

    EXPECT_THROW(calculator.setStrategy(nullptr), std::invalid_argument);
}

TEST(NotificationSubject, ReceivesEventAfterSubscribe)
{
    NotificationSubject subject;
    auto observer = std::make_shared<TestObserver>();

    subject.subscribe(observer);

    NotificationEvent event{ 1, "Created" };
    subject.notify(event);

    EXPECT_EQ(observer->updateCount, 1);
    EXPECT_EQ(observer->lastEvent.notificationId, 1);
    EXPECT_EQ(observer->lastEvent.newState, "Created");
}

TEST(NotificationSubject, DoesNotReceiveEventAfterUnsubscribe)
{
    NotificationSubject subject;
    auto observer = std::make_shared<TestObserver>();

    subject.subscribe(observer);
    subject.unsubscribe(observer);

    subject.notify({ 1, "Delivered" });

    EXPECT_EQ(observer->updateCount, 0);
}

TEST(NotificationSubject, NotifiesAllObservers)
{
    NotificationSubject subject;

    auto first = std::make_shared<TestObserver>();
    auto second = std::make_shared<TestObserver>();

    subject.subscribe(first);
    subject.subscribe(second);

    subject.notify({ 1, "Created" });

    EXPECT_EQ(first->updateCount, 1);
    EXPECT_EQ(second->updateCount, 1);
}

TEST(NotificationSubject, CountsOnlyDeliveredNotifications)
{
    DeliveryStatistics statistics;

    statistics.update({ 1, "Created" });
    statistics.update({ 2, "Sent" });
    statistics.update({ 3, "Delivered" });
    statistics.update({ 4, "Delivered" });

    EXPECT_EQ(statistics.getDeliveredCount(), 2);
}

TEST(NotificationSubject, RemovesExpiredObservers)
{
    NotificationSubject subject;

    {
        auto observer = std::make_shared<TestObserver>();
        subject.subscribe(observer);
    }

    EXPECT_NO_THROW(subject.notify({ 1, "Created" }));
}

TEST(PriorityFactory, MaximumFactoryCreatesWorkingStrategy)
{
    MaximumPriorityFactory factory;

    auto strategy = factory.createStrategy();

    ASSERT_NE(strategy, nullptr);

    Notification notification;
    notification.importance = 8;
    notification.urgency = 5;
    notification.waitingTime = 30;

    EXPECT_DOUBLE_EQ(strategy->calculate(notification), 8.0);
}

TEST(PriorityFactory, WeightedSumFactoryCreatesWorkingStrategy)
{
    WeightedSumPriorityFactory factory;

    auto strategy = factory.createStrategy();

    ASSERT_NE(strategy, nullptr);

    Notification notification;
    notification.importance = 8;
    notification.urgency = 5;
    notification.waitingTime = 30;

    EXPECT_DOUBLE_EQ(strategy->calculate(notification), 6.5);
}

TEST(PriorityFactory, NormalizedWeightedSumFactoryCreatesWorkingStrategy)
{
    NormalizedWeightedSumPriorityFactory factory;

    auto strategy = factory.createStrategy();

    ASSERT_NE(strategy, nullptr);

    Notification notification;
    notification.importance = 8;
    notification.urgency = 5;
    notification.waitingTime = 30;

    EXPECT_DOUBLE_EQ(strategy->calculate(notification), 6.5);
}