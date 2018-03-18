#ifndef NGSERVER_HEROINE_H
#define NGSERVER_HEROINE_H


#include <stack>

#include <glog/logging.h>
#include <cassert>

#include "Fsm.h"

enum class Input {
    PRESS_SPACE,
    PRESS_DOWN,
    RELEASE_DOWN,
    FIRE,
    PRESS_LEFT,
    RELEASE_LEFT,
    PRESS_RIGHT,
    RELEASE_RIGHT,
};

enum class Image {
    JUMP,
    DUCK,
    DIVE,
    STAND
};

static float const MAX_CHARGE = 2;

class Heroine;

class HeroineState {
public:
    virtual ~HeroineState() = default;

    virtual HeroineState *handleInput(Heroine &heroine, Input input) = 0;

    // virtual HeroineState *changeState() = 0;

    virtual void update(Heroine &heroine) = 0;

    virtual void enter(Heroine &heroine) = 0;

    virtual void exit(Heroine &heroine) = 0;

    virtual std::string name()= 0;
};

class DuckingState : public HeroineState {
public:
    DuckingState()
            : chargeTime_(0) {}

    explicit DuckingState(Heroine &heroine)
            : chargeTime_(0) {
        enter(heroine);
    }

    ~DuckingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

    std::string name() override;

private:
    float chargeTime_;
};

class StandingState : public HeroineState {
public:
    StandingState() = default;

    explicit StandingState(Heroine &heroine) {
        enter(heroine);
    }

    ~StandingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

    std::string name() override;
};

class JumpingState : public HeroineState {
public:
    JumpingState() = default;

    explicit JumpingState(Heroine &heroine) {
        enter(heroine);
    }

    ~JumpingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

    std::string name() override;

private:
    float jump_time_ = 0;
};

class DivingState : public HeroineState {
public:
    DivingState() = default;

    explicit DivingState(Heroine &heroine) {
        enter(heroine);
    }

    ~DivingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

    std::string name() override;
};

class MovingState : public HeroineState {
public:
    MovingState() = default;

    explicit MovingState(Heroine &heroine) {
        enter(heroine);
    }

    ~MovingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

    std::string name() override;
};

class EquipmentState : public HeroineState {
public:
    EquipmentState() = default;

    explicit EquipmentState(Heroine &heroine) {
        enter(heroine);
    }

    ~EquipmentState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

    std::string name() override;

    bool fire_ = false;

    float fire_interval_ = 0.0;
};

class HeroineEvent : public Fsm::Event {
public:
    HeroineEvent() = default;

    ~HeroineEvent() = default;

    Input input;
};

class Heroine : public Fsm::StateMachine<HeroineState> {
public:
    Heroine();

    void handleInput(Input input);

    void handleEvent(HeroineEvent *event) {
        handleInput(event->input);
        event = nullptr;
        delete event;
    }

    void handleEvent(Fsm::Event *event) override {

    };

    void update() override;

    HeroineState *getCurrentState() const override {
        return state_;
    };

    void changeState(HeroineState *state) override {
        state_ = state;
    };

    void superBomb();

    void setGraphics(Image image);

    std::string debugString() const override {
        return "current state " + state_->name();
    }

    HeroineState *state_;
    HeroineState *equipment_;

private:
    std::stack<HeroineState> states_;
};


#endif //NGSERVER_HEROINE_H
