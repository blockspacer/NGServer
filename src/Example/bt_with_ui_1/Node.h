#ifndef NGSERVER_NODE_H
#define NGSERVER_NODE_H


#include "TimeData.h"
#include "Status.h"

class Node {
public:
    virtual Status tick(TimeData time);
};


#endif //NGSERVER_NODE_H
