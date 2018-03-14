#include <benchmark/benchmark.h>

#include "../../Util/UUID.h"

using namespace Util;

static void BM_GetUuidString(benchmark::State &state) {
    for (auto _ : state) {
        std::string out{};
        UUID::GetUuidString(out);
    }
}

BENCHMARK(BM_GetUuidString);
