#include <benchmark/benchmark.h>

#include "../../Util/Crypt.h"

using namespace Util;

static void BM_GetMD5(benchmark::State &state) {
    for (auto _ : state) {
        std::string str{"abc"};
        std::string out{};
        Crypt::GetMD5(str, out);
    }
}

BENCHMARK(BM_GetMD5);

static void BM_DebugGetMD5(benchmark::State &state) {
    for (auto _ : state) {
        std::string str{"abc"};
        std::string out{};
        Crypt::DebugGetMD5(str, out);
    }
}

BENCHMARK(BM_DebugGetMD5);

static void BM_GetSha1(benchmark::State &state) {
    for (auto _ : state) {
        std::string str{"abc"};
        std::string out{};
        Crypt::GetSha1(str, out);
    }
}

BENCHMARK(BM_GetSha1);

static void BM_DebugGetSha1(benchmark::State &state) {
    for (auto _ : state) {
        std::string str{"abc"};
        std::string out{};
        Crypt::DebugGetSha1(str, out);
    }
}

BENCHMARK(BM_DebugGetSha1);
