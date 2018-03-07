#include <gtest/gtest.h>
#include <glog/logging.h>

#include "../../Util/TimeStamp.h"

using namespace Util;

TEST(TimeStamp, Temp) {
    LOG(INFO) << Util::TimeStamp::GetTimeStamp();
    LOG(INFO) << Util::TimeStamp::GetTimeStampMs();
}