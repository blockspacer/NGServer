#include <benchmark/benchmark.h>

#include "../../Util/TimeStamp.h"

using namespace Util;

static void BM_GetTimeStamp(benchmark::State &state) {
    for (auto _ : state) {
        Util::TimeStamp::GetTimeStamp();
    }
}

BENCHMARK(BM_GetTimeStamp);

static void BM_GetTimeStampUs(benchmark::State &state) {
    for (auto _ : state) {
        Util::TimeStamp::GetTimeStampUs();
    }
}

BENCHMARK(BM_GetTimeStampUs);

static void BM_GetTimeStampMs(benchmark::State &state) {
    for (auto _ : state) {
        Util::TimeStamp::GetTimeStampMs();
    }
}

BENCHMARK(BM_GetTimeStampMs);
