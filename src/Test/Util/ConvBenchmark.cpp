#include <benchmark/benchmark.h>

#include "../../Util/Conv.h"

using namespace Util;

static void BM_StringToChar(benchmark::State &state) {
    for (auto _ : state) {
        std::string str{"hello world"};
        char *c;
        c = StringToChar(str);
    }
}

BENCHMARK(BM_StringToChar);

static void BM_StringToInt(benchmark::State &state) {
    for (auto _ : state) {
        std::string str{"123456789"};
        int out;
        out = StringToInt(str);
    }
}

BENCHMARK(BM_StringToInt);

static void BM_SerializeInt32(benchmark::State &state) {
    for (auto _ : state) {
        char buff[4];
        int size{99};
        SerializeInt32(buff, size);
    }
}

BENCHMARK(BM_SerializeInt32);

static void BM_ParseInt32(benchmark::State &state) {
    for (auto _ : state) {
        char buff[4];
        buff[0] = 'c';
        buff[1] = '\0';
        buff[2] = '\0';
        buff[3] = '\0';
        int size{0};
        size = ParseInt32(buff);
    }
}

BENCHMARK(BM_ParseInt32);
