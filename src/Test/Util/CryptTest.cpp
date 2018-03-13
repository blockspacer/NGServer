#include <gtest/gtest.h>
#include <glog/logging.h>

#include "../../Util/Crypt.h"

using namespace Util;

TEST(Crypt, Temp) {


    std::string str1{"abc"};
    std::string str2{"abc"};
    std::string str3{"abc"};
    std::string str4{"abc"};
    std::string out1{};
    std::string out2{};
    std::string out3{};
    std::string out4{};

    Crypt::GetMD5(str1, out1);
    ASSERT_EQ(out1, "900150983cd24fb0d6963f7d28e17f72");

    Crypt::DebugGetMD5(str2, out1);
    ASSERT_EQ(out1, "900150983cd24fb0d6963f7d28e17f72");

    Crypt::GetSha1(str3, out3);
    ASSERT_EQ(out3, "a9993e364706816aba3e25717850c26c9cd0d89d");

    Crypt::DebugGetSha1(str4, out4);
    ASSERT_EQ(out4, "a9993e364706816aba3e25717850c26c9cd0d89d");
}