#ifndef NGSERVER_TIMESTAMP_H
#define NGSERVER_TIMESTAMP_H


#include <zconf.h>
#include <sys/time.h>

namespace Util {

class TimeStamp {
public:
    /**
     * @brief Get current timestamp
     * @return second eg: 1520988647
     */
    static int64_t GetTimeStamp() {
        timeval tv = {};
        gettimeofday(&tv, nullptr);
        return tv.tv_sec;
    }

    /**
     * @brief Get current timestamp
     * @return millisecond eg: 1520988647120, 120 is millisecond
     */
    static int64_t GetTimeStampMs() {
        timeval tv = {};
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }

    /**
     * @brief Get current timestamp
     * @return millisecond eg: 1520989267497662, 497662 is microsecond
     */
    static int64_t GetTimeStampUs() {
        timeval tv = {};
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
    }
};

}


#endif //NGSERVER_TIMESTAMP_H
