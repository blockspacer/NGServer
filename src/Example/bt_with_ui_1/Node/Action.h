#ifndef NGSERVER_ACTION_H
#define NGSERVER_ACTION_H


#include <string>

#include "../Node.h"

typedef Status (*Func)(TimeData);

/**
 * @brief A behaviour tree leaf node for running an action.
 */
class Action : public Node {
public:
    explicit Action(std::string name, Func fn) {
        name_ = name;
        fn_ = fn;
    }

private:
    std::string name_;
    Func fn_;
};


#endif //NGSERVER_ACTION_H
