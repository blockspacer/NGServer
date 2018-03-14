#include <gtest/gtest.h>

#include "../../Util/TimeStamp.h"

using namespace Util;

// 2018/3/14 9:7:56 timestamp 1520989676
// 2038/1/19 11:14:7 timestamp 2147483647

TEST(TimeStamp, GetTimeStamp) {
    ASSERT_LE(1520989676, Util::TimeStamp::GetTimeStamp());
    ASSERT_GE(2147483647, Util::TimeStamp::GetTimeStamp());
}

TEST(TimeStamp, GetTimeStampMs) {
    ASSERT_LE(1520989676220, Util::TimeStamp::GetTimeStampMs());
    ASSERT_GE(2147483647000, Util::TimeStamp::GetTimeStampMs());
}

TEST(TimeStamp, GetTimeStampUs) {
    ASSERT_LE(1520989676220990, Util::TimeStamp::GetTimeStampUs());
    ASSERT_GE(2147483647000000, Util::TimeStamp::GetTimeStampUs());
}