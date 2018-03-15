#include "Heroine.h"

StandingState HeroineState::standing = {};
JumpingState HeroineState::jumping = {};
DuckingState HeroineState::ducking = {};
DivingState HeroineState::diving = {};

HeroineState *DuckingState::handleInput(Heroine &heroine, Input input) {
    if (input == Input::RELEASE_DOWN) {
        heroine.state_ = &HeroineState::standing;
        return new StandingState;
    }
    return nullptr;
}

void DuckingState::update(Heroine &heroine) {
    chargeTime_++;
    if (chargeTime_ > MAX_CHARGE) {
        heroine.superBomb();
    }
}

void DuckingState::enter(Heroine &heroine) {
    heroine.setGraphics(Image::DUCK);
}

void DuckingState::exit(Heroine &heroine) {

}

HeroineState *StandingState::handleInput(Heroine &heroine, Input input) {
    if (input == Input::PRESS_B) {
        heroine.state_ = &HeroineState::jumping;
        heroine.setGraphics(Image::JUMP);
        return new JumpingState();
    } else if (input == Input::PRESS_DOWN) {
        heroine.state_ = &HeroineState::ducking;
        heroine.setGraphics(Image::DUCK);
        return new DuckingState();
    }
    return nullptr;
}

void StandingState::update(Heroine &heroine) {

}

void StandingState::enter(Heroine &heroine) {
    heroine.setGraphics(Image::STAND);
}

void StandingState::exit(Heroine &heroine) {

}

HeroineState *JumpingState::handleInput(Heroine &heroine, Input input) {
    if (input == Input::PRESS_DOWN) {
        heroine.state_ = &HeroineState::diving;
        heroine.setGraphics(Image::DIVE);
        return new DivingState();
    }
    return nullptr;
}

void JumpingState::update(Heroine &heroine) {

}

void JumpingState::enter(Heroine &heroine) {
    heroine.setGraphics(Image::JUMP);
}

void JumpingState::exit(Heroine &heroine) {

}

HeroineState *DivingState::handleInput(Heroine &heroine, Input input) {
    return new DivingState();
}

void DivingState::update(Heroine &heroine) {

}

void DivingState::enter(Heroine &heroine) {
    heroine.setGraphics(Image::DIVE);
}

void DivingState::exit(Heroine &heroine) {

}

