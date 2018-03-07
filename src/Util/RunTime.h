#ifndef MASSGATE_MOD_MC_TIME_H
#define MASSGATE_MOD_MC_TIME_H


#include <ctime>

namespace Util {

class RunTime {
public:
    RunTime();

    ~RunTime();

    void Start();

    void End();

    timeval GetStartTime();

    timeval GetEndTime();

    long double GetRunTime();

private:
    timeval start_;
    timeval end_;
};

}


#endif //MASSGATE_MOD_MC_TIME_H
