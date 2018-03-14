#include <gtest/gtest.h>

#include "../../Util/UUID.h"

using namespace Util;

TEST(Crypt, GetUuidString) {
    std::string str{"abc"};
    std::string out{};
    UUID::GetUuidString(out);
    ASSERT_EQ(36, out.length());
}