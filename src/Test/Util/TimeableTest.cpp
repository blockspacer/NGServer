#include <gtest/gtest.h>
#include <glog/logging.h>

#include "../../Util/Timeable.h"

using namespace Util;

TEST(Timeable, Timer_Multiple_Total_Avg) {
    auto ta = Timeable();
    double precision = 0.02;
    ta.BeginTimer(Timeable::TIMER_ONE);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_ONE);
    ASSERT_GE(1 + precision, ta.GetTotalTime(Timeable::TIMER_ONE));
    ASSERT_LE(1 - precision, ta.GetTotalTime(Timeable::TIMER_ONE));

    sleep(1);
    ta.EndTimer(Timeable::TIMER_ONE);
    ASSERT_GE(3 + precision, ta.GetTotalTime(Timeable::TIMER_ONE));
    ASSERT_LE(3 - precision, ta.GetTotalTime(Timeable::TIMER_ONE));

    sleep(1);
    ta.EndTimer(Timeable::TIMER_ONE);
    ASSERT_GE(6 + precision, ta.GetTotalTime(Timeable::TIMER_ONE));
    ASSERT_LE(6 - precision, ta.GetTotalTime(Timeable::TIMER_ONE));

    sleep(1);
    ta.EndTimer(Timeable::TIMER_ONE);
    ASSERT_GE(10 + precision, ta.GetTotalTime(Timeable::TIMER_ONE));
    ASSERT_LE(10 - precision, ta.GetTotalTime(Timeable::TIMER_ONE));

    sleep(1);
    ta.EndTimer(Timeable::TIMER_ONE);
    ASSERT_GE(15 + precision, ta.GetTotalTime(Timeable::TIMER_ONE));
    ASSERT_LE(15 - precision, ta.GetTotalTime(Timeable::TIMER_ONE));

    sleep(1);
    ta.EndTimer(Timeable::TIMER_ONE);
    ASSERT_GE(21 + precision, ta.GetTotalTime(Timeable::TIMER_ONE));
    ASSERT_LE(21 - precision, ta.GetTotalTime(Timeable::TIMER_ONE));

    ASSERT_GE(3.5 + precision, ta.GetAverageTime(Timeable::TIMER_ONE));
    ASSERT_LE(3.5 - precision, ta.GetAverageTime(Timeable::TIMER_ONE));
}

TEST(Timeable, TIMER_ONE) {
    auto ta = Timeable();
    ta.BeginTimer(Timeable::TIMER_ONE);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_ONE);
    ASSERT_GE(1.01, ta.GetTotalTime(Timeable::TIMER_ONE));
    ASSERT_LE(0.99, ta.GetTotalTime(Timeable::TIMER_ONE));
}

TEST(Timeable, TIMER_TWO) {
    auto ta = Timeable();
    ta.BeginTimer(Timeable::TIMER_TWO);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_TWO);
    ASSERT_GE(1.01, ta.GetTotalTime(Timeable::TIMER_TWO));
    ASSERT_LE(0.99, ta.GetTotalTime(Timeable::TIMER_TWO));
}

TEST(Timeable, TIMER_THREE) {
    auto ta = Timeable();
    ta.BeginTimer(Timeable::TIMER_THREE);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_THREE);
    ASSERT_GE(1.01, ta.GetTotalTime(Timeable::TIMER_THREE));
    ASSERT_LE(0.99, ta.GetTotalTime(Timeable::TIMER_THREE));
}

TEST(Timeable, TIMER_FOUR) {
    auto ta = Timeable();
    ta.BeginTimer(Timeable::TIMER_FOUR);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_FOUR);
    ASSERT_GE(1.01, ta.GetTotalTime(Timeable::TIMER_FOUR));
    ASSERT_LE(0.99, ta.GetTotalTime(Timeable::TIMER_FOUR));
}

