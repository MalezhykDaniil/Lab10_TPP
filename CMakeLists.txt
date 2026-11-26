cmake_minimum_required(VERSION 3.21)
project(NotificationORM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SqliteOrm CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(NotificationORM
    src/main.cpp
    src/SQLiteNotificationCategoryRepository.cpp
    src/TestData.cpp
    src/CreatedState.cpp
    src/SentState.cpp
    src/DeliveredState.cpp
    src/DeliveryErrorState.cpp
    src/NotificationStateRepository.cpp
    src/MaximumPriorityStrategy.cpp
    src/WeightedPriorityStrategy.cpp
    src/NormalizedWeightedSumStrategy.cpp
    src/PriorityCalculator.cpp
    src/NotificationSubject.cpp
    src/NotificationLogger.cpp
    src/DeliveryStatistics.cpp
    src/MaximumPriorityFactory.cpp
    src/WeightedSumPriorityFactory.cpp
    src/NormalizedWeightedSumPriorityFactory.cpp
    include/INotificationState.h
    include/CreatedState.h
    include/SentState.h
    include/DeliveredState.h
    include/DeliveryErrorState.h
    include/NotificationContext.h
    include/NotificationStateHistory.h
    include/NotificationStateRepository.h
    include/INotificationStateRepository.h
    include/IPriorityStrategy.h
    include/MaximumPriorityStrategy.h
    include/WeightedSumPriorityStrategy.h
    include/NormalizedWeightedSumPriorityStrategy.h
    include/PriorityCalculator.h
    include/NotificationEvent.h
    include/INotificationObserver.h
    include/NotificationSubject.h
    include/NotificationLogger.h
    include/DeliveryStatistics.h
    include/IPriorityStrategyFactory.h
    include/MaximumPriorityFactory.h
    include/WeightedSumPriorityFactory.h
    include/NormalizedWeightedSumPriorityFactory.h
)

target_include_directories(NotificationORM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(NotificationORM PRIVATE
    sqlite_orm::sqlite_orm
    unofficial::sqlite3::sqlite3
)

enable_testing()

add_executable(NotificationORMTests
    tests/NotificationORMTest.cpp
    src/CreatedState.cpp
    src/SentState.cpp
    src/DeliveredState.cpp
    src/DeliveryErrorState.cpp
    src/NotificationStateRepository.cpp
    src/MaximumPriorityStrategy.cpp
    src/WeightedPriorityStrategy.cpp
    src/NormalizedWeightedSumStrategy.cpp
    src/PriorityCalculator.cpp
    src/NotificationSubject.cpp
    src/NotificationLogger.cpp
    src/DeliveryStatistics.cpp
    src/MaximumPriorityFactory.cpp
    src/WeightedSumPriorityFactory.cpp
    src/NormalizedWeightedSumPriorityFactory.cpp
)

target_include_directories(NotificationORMTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(NotificationORMTests PRIVATE
    GTest::gtest_main
    sqlite_orm::sqlite_orm
    unofficial::sqlite3::sqlite3
)

include(GoogleTest)
gtest_discover_tests(NotificationORMTests)