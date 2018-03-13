#include <gtest/gtest.h>
#include <glog/logging.h>

#include "../../Util/Crypt.h"

using namespace Util;

TEST(Crypt, Temp) {
    std::string str{"abc"};
    std::string out{};

    out.clear();
    Crypt::GetMD5(str, out);
    ASSERT_EQ(out, "900150983cd24fb0d6963f7d28e17f72");

    out.clear();
    Crypt::DebugGetMD5(str, out);
    ASSERT_EQ(out, "900150983cd24fb0d6963f7d28e17f72");

    out.clear();
    Crypt::GetSha1(str, out);
    ASSERT_EQ(out, "a9993e364706816aba3e25717850c26c9cd0d89d");

    out.clear();
    Crypt::DebugGetSha1(str, out);
    ASSERT_EQ(out, "a9993e364706816aba3e25717850c26c9cd0d89d");
}