#ifndef NGSERVER_HEROINE_H
#define NGSERVER_HEROINE_H

enum class State {
    STANDING, // DUCKING -> RELEASE_DOWN
    JUMPING,  // STANDING -> PRESS_B
    DUCKING,  // STANDING -> PRESS_DOWN
    DIVING    // JUMPING -> PRESS_DOWN
};

enum class Input {
    PRESS_B,
    PRESS_DOWN,
    RELEASE_DOWN
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

class HeroineState {
public:
    static StandingState standing;
    static DuckingState ducking;
    static JumpingState jumping;
    static DivingState diving;

    virtual ~HeroineState() = default;

    virtual HeroineState *handleInput(Heroine &heroine, Input input) = 0;

    virtual void update(Heroine &heroine) = 0;

    virtual void enter(Heroine &heroine) = 0;

    virtual void exit(Heroine &heroine) = 0;
};

class DuckingState : public HeroineState {
public:
    DuckingState() : chargeTime_(0) {}

    ~DuckingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;

private:
    int chargeTime_;
};

class StandingState : public HeroineState {
public:
    StandingState() = default;

    ~StandingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;
};

class JumpingState : public HeroineState {
public:
    JumpingState() = default;

    ~JumpingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;
};

class DivingState : public HeroineState {
public:
    DivingState() = default;

    ~DivingState() override = default;

    HeroineState *handleInput(Heroine &heroine, Input input) override;

    void update(Heroine &heroine) override;

    void enter(Heroine &heroine) override;

    void exit(Heroine &heroine) override;
};

class Heroine {
public:
    virtual void handleInput(Input input) {
        HeroineState *state = state_->handleInput(*this, input);
        if (state != nullptr) {
            delete state_;
            state_ = state;
        }
    }

    virtual void update() {
        state_->update(*this);
    }

    void superBomb() {}

    void setGraphics(Image image) {}

    HeroineState *state_ = {};
private:

};


#endif //NGSERVER_HEROINE_H
