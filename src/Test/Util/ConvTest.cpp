#include <gtest/gtest.h>
#include <glog/logging.h>

#include "../../Util/Conv.h"

using namespace Util;

TEST(Conv, StringToChar) {
    std::string str{"hello world"};
    char *c;
    c = StringToChar(str);
    ASSERT_STREQ(c, "hello world");
}

TEST(Conv, StringToInt) {
    std::string str{"123456789"};
    int out;
    out = StringToInt(str);
    ASSERT_EQ(out, 123456789);
}

TEST(Conv, SerializeInt32) {
    char buff[4];
    int size{99};
    SerializeInt32(buff, size);
    EXPECT_EQ(buff[0], 'c');
    EXPECT_EQ(buff[1], '\0');
    EXPECT_EQ(buff[2], '\0');
    EXPECT_EQ(buff[3], '\0');
}

TEST(Conv, ParseInt32) {
    char buff[4];
    buff[0] = 'c';
    buff[1] = '\0';
    buff[2] = '\0';
    buff[3] = '\0';
    int size{0};
    size = ParseInt32(buff);
    ASSERT_EQ(size, 99);
}