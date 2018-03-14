#include <gtest/gtest.h>
#include <glog/logging.h>

#include "../../Util/Timeable.h"

using namespace Util;

TEST(DISABLED_Timeable, Temp) {
    auto ta = Timeable();

    ta.BeginTimer(Timeable::TIMER_ONE);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_ONE);
    LOG(INFO) << ta.GetTotalTime(Timeable::TIMER_ONE) << " second TIMER_ONE";

    ta.BeginTimer(Timeable::TIMER_TWO);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_TWO);
    LOG(INFO) << ta.GetTotalTime(Timeable::TIMER_TWO) << " second TIMER_TWO";

    ta.BeginTimer(Timeable::TIMER_THREE);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_THREE);
    LOG(INFO) << ta.GetTotalTime(Timeable::TIMER_THREE) << " second TIMER_THREE";

    ta.BeginTimer(Timeable::TIMER_FOUR);
    sleep(1);
    ta.EndTimer(Timeable::TIMER_FOUR);
    LOG(INFO) << ta.GetTotalTime(Timeable::TIMER_FOUR) << " second TIMER_FOUR [1] (1) = 1";

    sleep(1);
    ta.EndTimer(Timeable::TIMER_FOUR);
    LOG(INFO) << ta.GetTotalTime(Timeable::TIMER_FOUR) << " second TIMER_FOUR [2] (1 + 2) = 3";

    sleep(1);
    ta.EndTimer(Timeable::TIMER_FOUR);
    LOG(INFO) << ta.GetTotalTime(Timeable::TIMER_FOUR)
              << " second TIMER_FOUR [3] (1 + 2 + 3) = 6";

    LOG(INFO) << ta.GetTotalTime(Timeable::TIMER_FOUR)
              << " second TIMER_FOUR total "
              << "(1 + 2 + 3) = 6";

    LOG(INFO) << ta.GetAverageTime(Timeable::TIMER_FOUR) << " second TIMER_FOUR avg";
}