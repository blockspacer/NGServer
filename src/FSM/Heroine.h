#ifndef NGSERVER_HEROINE_H
#define NGSERVER_HEROINE_H


#include <glog/logging.h>

// case '1':
// hero1.handleInput(Input::PRESS_DOWN);
// case '2':
// hero1.handleInput(Input::RELEASE_DOWN);
// case '3':
// hero1.handleInput(Input::PRESS_B);

enum class State {
    STANDING, // DUCKING -> RELEASE_DOWN
    JUMPING,  // STANDING -> PRESS_B
    DUCKING,  // STANDING -> PRESS_DOWN
    DIVING    // JUMPING -> PRESS_DOWN
};

enum class Input {
    PRESS_B,
    PRESS_DOWN,
    RELEASE_DOWN,
    FIRE
};

enum class Image {
    JUMP,
    DUCK,
    DIVE,
    STAND
};

static int const MAX_CHARGE = 0;

class Heroine;

class StandingState;

class DuckingState;

class JumpingState;

class DivingState;

class EquipmentState;

class HeroineState {
public:
    static StandingState standing;
    static DuckingState ducking;
    static JumpingState jumping;
    static DivingState diving;
    static EquipmentState equipment;

    virtual ~HeroineState() = default;

    virtual HeroineState *handleInput(Heroine &heroine, Input input) = 0;

    virtual void update(Heroine &heroine) = 0;

    virtual void enter(Heroine &heroine) = 0;

    virtual void exit(Heroine &heroine) = 0;

    virtual std::string name()= 0;
};

class DuckingState : public HeroineState {
public:
    DuckingState() : chargeTime_(0) {}

    explicit DuckingState(Heroine &heroine) {
        enter(heroine);
    }

    ~DuckingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

    std::string name() override {
        return "DuckingState";
    }

private:
    int chargeTime_;
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

    std::string name() override {
        return "StandingState";
    }
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

    std::string name() override {
        return "JumpingState";
    }
};

class DivingState : public HeroineState {
public:
    DivingState()= default;

    explicit DivingState(Heroine &heroine) {
        enter(heroine);
    }

    ~DivingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

    std::string name() override {
        return "DivingState";
    }
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

    std::string name() override {
        return "EquipmentState";
    }
};

class Heroine {
public:
    Heroine() {
        state_ = &HeroineState::standing;
        equipment_ = &HeroineState::equipment;
    };

    virtual void handleInput(Input input) {
        HeroineState *state = state_->handleInput(*this, input);
        equipment_->handleInput(*this, input);
        if (state != nullptr) {
            // delete state_;
            state_ = state;
        }
    }

    virtual void update() {
        state_->update(*this);
    }

    void superBomb() { LOG(INFO) << "superBomb"; }

    void setGraphics(Image image) { LOG(INFO) << "setGraphics"; }

    HeroineState *state_;
    HeroineState *equipment_;
private:

};


#endif //NGSERVER_HEROINE_H
