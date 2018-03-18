#ifndef NGSERVER_FSM_H
#define NGSERVER_FSM_H


#include <stack>

namespace Fsm {

class Event {
public:
    virtual ~Event() = default;
};

class State;

template<typename S>
class StateMachine {
public:
    virtual ~StateMachine() = default;

    virtual void update() = 0;

    virtual void handleEvent(Event *event) = 0;

    virtual void changeState(S *state) = 0;

    virtual S *getCurrentState() const =0;

    virtual std::string debugString() const =0;

    State *popState() {
        auto result = states_.top();
        states_.pop();
        return result;
    }

    void pushState(State *state) {
        states_.push(state);
    }

private:
    std::stack<State *> states_;
};


class State {
public:
    virtual ~State() = default;

    virtual void handleEvent() = delete;

    virtual void update() = 0;

    virtual void enter() =0;

    virtual void exit() = 0;

    virtual bool condition() = delete;

    virtual std::string debugString() = delete;

    virtual std::string debugName() = delete;
};

}


#endif //NGSERVER_FSM_H
