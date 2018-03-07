#ifndef MASSGATE_MOD_TIMEABLE_H
#define MASSGATE_MOD_TIMEABLE_H

namespace Util {

class Timeable {
public:
    Timeable();

    Timeable(const Timeable &aRhs);

    virtual ~Timeable() = default;

    typedef enum {
        TIMER_ONE, TIMER_TWO, TIMER_THREE, TIMER_FOUR, NUM_TIMER_INDEXES
    } TimerIndex;

    void BeginTimer(TimerIndex timer);

    void EndTimer(TimerIndex timer);

    double GetTotalTime(TimerIndex theTimer);

    double GetAverageTime(TimerIndex theTimer);

    Timeable &operator=(const Timeable &aRhs);

private:
    unsigned long last_begin_time_[NUM_TIMER_INDEXES];
    unsigned long number_of_timers_[NUM_TIMER_INDEXES];
    double total_time_[NUM_TIMER_INDEXES];
};

}


#endif //MASSGATE_MOD_TIMEABLE_H
