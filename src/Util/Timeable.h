#ifndef NGSERVER_TIMEABLE_H
#define NGSERVER_TIMEABLE_H

namespace Util {

class Timeable {
public:
    Timeable();

    Timeable(const Timeable &aRhs);

    virtual ~Timeable() = default;

    typedef enum {
        TIMER_ONE, TIMER_TWO, TIMER_THREE, TIMER_FOUR, NUM_TIMER_INDEXES
    } TimerIndex;

    /**
     * @brief Record the current time as begin time
     * @param timer
     */
    void BeginTimer(TimerIndex timer);

    /**
     * @brief Record the current time as end time
     *        Call this will cause number_of_timers add 1
     * @param timer
     */
    void EndTimer(TimerIndex timer);

    /**
     * @brief Get total time by (end time - begin time)
     * @param timer
     * @return
     */
    double GetTotalTime(TimerIndex timer);

    /**
     * @brief Get Avg time by (end time - begin time)/ number_of_timers
     * @param timer
     * @return
     */
    double GetAverageTime(TimerIndex timer);

    Timeable &operator=(const Timeable &aRhs);

private:
    unsigned long last_begin_time_[NUM_TIMER_INDEXES];
    unsigned long number_of_timers_[NUM_TIMER_INDEXES];
    double total_time_[NUM_TIMER_INDEXES];
};

}


#endif //NGSERVER_TIMEABLE_H
