#ifndef MASSGATE_MOD_TIMESTAMP_H
#define MASSGATE_MOD_TIMESTAMP_H


#include <zconf.h>
#include <sys/time.h>

namespace Util {

class TimeStamp {
public:
    static int64_t GetTimeStamp() {
        timeval tv = {};
        gettimeofday(&tv, nullptr);
        return tv.tv_sec;
    }

    static int64_t GetTimeStampMs() {
        timeval tv = {};
        gettimeofday(&tv, nullptr);
        return tv.tv_sec * 1000 + tv.tv_usec / 1000.0;
    }
};

}


#endif //MASSGATE_MOD_TIMESTAMP_H
