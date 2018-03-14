#include <gtest/gtest.h>
#include <glog/logging.h>
#include <benchmark/benchmark.h>

#include "../../Util/Timeable.h"

using namespace Util;

static void BM_Timeable(benchmark::State &state) {
    for (auto _ : state) {
        auto ta = Timeable();
    }
}

BENCHMARK(BM_Timeable);

static void BM_Timeable_Timer_one(benchmark::State &state) {
    for (auto _ : state) {
        auto ta = Timeable();
        ta.BeginTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
    }
}

BENCHMARK(BM_Timeable_Timer_one);

static void BM_Timeable_Timer_four(benchmark::State &state) {
    for (auto _ : state) {
        auto ta = Timeable();
        ta.BeginTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.BeginTimer(Timeable::TIMER_TWO);
        ta.EndTimer(Timeable::TIMER_TWO);
        ta.BeginTimer(Timeable::TIMER_THREE);
        ta.EndTimer(Timeable::TIMER_THREE);
        ta.BeginTimer(Timeable::TIMER_FOUR);
        ta.EndTimer(Timeable::TIMER_FOUR);
    }
}

BENCHMARK(BM_Timeable_Timer_four);

static void BM_Timeable_Timer_four_total(benchmark::State &state) {
    for (auto _ : state) {
        auto ta = Timeable();
        ta.BeginTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.GetTotalTime(Timeable::TIMER_FOUR);
        ta.GetAverageTime(Timeable::TIMER_FOUR);
    }
}

BENCHMARK(BM_Timeable_Timer_four_total);

static void BM_Timeable_Timer_four_average(benchmark::State &state) {
    for (auto _ : state) {
        auto ta = Timeable();
        ta.BeginTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.EndTimer(Timeable::TIMER_ONE);
        ta.GetAverageTime(Timeable::TIMER_FOUR);
    }
}

BENCHMARK(BM_Timeable_Timer_four_average);
