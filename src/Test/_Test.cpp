#include <glog/logging.h>
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

int main(int argc, char *argv[]) {

    testing::InitGoogleTest(&argc, argv);

    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    FLAGS_colorlogtostderr = true;

    // LOG(INFO) << "Running gtest in Test directory";
    auto result = RUN_ALL_TESTS();

    // LOG(INFO) << "Running benchmark in Test directory";

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();

    return result;
}

// BENCHMARK_MAIN();