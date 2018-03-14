#include "Timeable.h"

#include <cassert>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include <iostream>

namespace {
const double PRECISION_MS = 1000.0 * 1000.0;    // millisecond
const double PRECISION_US = 1000.0;             // microsecond
const double PRECISION = PRECISION_MS;          // use millisecond precision
}

namespace Util {

Timeable::Timeable() {
    memset(last_begin_time_, 0, sizeof(last_begin_time_));
    memset(number_of_timers_, 0, sizeof(number_of_timers_));
    for (auto &item :total_time_) {
        item = 0.0;
    }
}

void Timeable::BeginTimer(TimerIndex timer) {
    assert(timer < NUM_TIMER_INDEXES);
    timeval tv = {};
    gettimeofday(&tv, nullptr);
    last_begin_time_[timer] = (unsigned long) (tv.tv_sec * PRECISION + tv.tv_usec);
}

void Timeable::EndTimer(TimerIndex timer) {
    assert(timer < NUM_TIMER_INDEXES);
    number_of_timers_[timer]++;
    timeval tv = {};
    gettimeofday(&tv, nullptr);
    total_time_[timer] += (tv.tv_sec * PRECISION + tv.tv_usec - last_begin_time_[timer]);
}

double Timeable::GetTotalTime(TimerIndex timer) {
    assert(timer < NUM_TIMER_INDEXES);
    return (total_time_[timer]) / PRECISION;
}

double Timeable::GetAverageTime(TimerIndex timer) {
    assert(timer < NUM_TIMER_INDEXES);
    return (total_time_[timer] / PRECISION) / double(number_of_timers_[timer]);
}

Timeable::Timeable(const Timeable &timer) {
    *this = timer;
}

Timeable &Timeable::operator=(const Timeable &timer) {
    memcpy(last_begin_time_, timer.last_begin_time_, sizeof(last_begin_time_));
    memcpy(number_of_timers_, timer.number_of_timers_, sizeof(number_of_timers_));
    memcpy(total_time_, timer.total_time_, sizeof(total_time_));
    return *this;
}

}
