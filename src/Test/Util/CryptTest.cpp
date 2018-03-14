#include <gtest/gtest.h>

#include "../../Util/Crypt.h"

using namespace Util;

TEST(Crypt, GetMD5) {
    std::string str{"abc"};
    std::string out{};
    Crypt::GetMD5(str, out);
    ASSERT_EQ(out, "900150983cd24fb0d6963f7d28e17f72");
}

TEST(Crypt, DebugGetMD5) {
    std::string str{"abc"};
    std::string out{};
    Crypt::DebugGetMD5(str, out);
    ASSERT_EQ(out, "900150983cd24fb0d6963f7d28e17f72");
}

TEST(Crypt, GetSha1) {
    std::string str{"abc"};
    std::string out{};
    Crypt::GetSha1(str, out);
    ASSERT_EQ(out, "a9993e364706816aba3e25717850c26c9cd0d89d");
}

TEST(Crypt, DebugGetSha1) {
    std::string str{"abc"};
    std::string out{};
    Crypt::DebugGetSha1(str, out);
    ASSERT_EQ(out, "a9993e364706816aba3e25717850c26c9cd0d89d");
}