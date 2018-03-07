#include <gtest/gtest.h>
#include <glog/logging.h>

#include "../../Util/Crypt.h"

using namespace Util;

TEST(Crypt, Temp) {
    ASSERT_EQ(Crypt::GetMD5("abc"), "900150983CD24FB0D6963F7D28E17F72");
    ASSERT_EQ(Crypt::GetSha1("abc"), "a9993e364706816aba3e25717850c26c9cd0d89d");
}