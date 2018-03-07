#ifndef NGSERVER_MC_TIME_H
#define NGSERVER_MC_TIME_H


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


#endif //NGSERVER_MC_TIME_H
