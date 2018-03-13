#include <glog/logging.h>
#include <gtest/gtest.h>

#include "../Util/Crypt.h"

GTEST_API_ int main(int argc, char *argv[]) {

    testing::InitGoogleTest(&argc, argv);

    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;

    LOG(INFO) << "Running gtest in Test directory";

    return RUN_ALL_TESTS();
}
