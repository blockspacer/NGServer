#ifndef NGSERVER_FSM_H
#define NGSERVER_FSM_H


/**
 * @brief Finate State Machine Design class
 *
 * FsmState     class
 * FsmMachie         class
 * FsmEventDate    class
 */

class FsmEventData {
public:
    virtual ~FsmEventData() = default;
};

class FsmState {
public:
    virtual ~FsmState() = default;

    template<typename E, typename S>
    S *handleEvent(E &) {};

    virtual void update() = 0;

    virtual void enter() =0;

    virtual void exit() = 0;

    virtual void transition() = 0;

    virtual bool condition() = 0;

    virtual std::string debugString() = 0;

    virtual std::string debugName() = 0;
};

class FsmMachine {
public:
    virtual ~FsmMachine() = default;

    void popState() {}

    void pushState() {}

    void getCurrentState() {}

private:
};


#endif //NGSERVER_FSM_H
