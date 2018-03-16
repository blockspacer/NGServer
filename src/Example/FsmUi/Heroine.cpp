#include "Heroine.h"

StandingState HeroineState::standing = {};
JumpingState HeroineState::jumping = {};
DuckingState HeroineState::ducking = {};
DivingState HeroineState::diving = {};
EquipmentState HeroineState::equipment = {};

HeroineState *DuckingState::handleInput(Heroine &heroine, Input input) {
    if (input == Input::RELEASE_DOWN) {
        exit(heroine);
        return new StandingState(heroine);
    }
    return nullptr;
}

void DuckingState::update(Heroine &heroine) {
    chargeTime_ += 0.1;
    if (chargeTime_ > MAX_CHARGE) {
        heroine.superBomb();
    }
}

void DuckingState::enter(Heroine &heroine) {
    LOG(INFO) << "enter " << name();
    heroine.setGraphics(Image::DUCK);
}

void DuckingState::exit(Heroine &heroine) {
    LOG(INFO) << "exit " << name();
}

std::string DuckingState::name() {
    return "DuckingState";
}

HeroineState *StandingState::handleInput(Heroine &heroine, Input input) {
    if (input == Input::PRESS_SPACE) {
        exit(heroine);
        return new JumpingState(heroine);
    } else if (input == Input::PRESS_DOWN) {
        exit(heroine);
        return new DuckingState(heroine);
    } else if (input == Input::PRESS_LEFT || input == Input::PRESS_RIGHT) {
        exit(heroine);
        return new MovingState(heroine);
    }
    return nullptr;
}

void StandingState::update(Heroine &heroine) {

}

void StandingState::enter(Heroine &heroine) {
    LOG(INFO) << "enter " << name();
    heroine.setGraphics(Image::STAND);
}

void StandingState::exit(Heroine &heroine) {
    LOG(INFO) << "exit " << name();
}

std::string StandingState::name() {
    return "StandingState";
}

HeroineState *JumpingState::handleInput(Heroine &heroine, Input input) {
    if (input == Input::PRESS_DOWN) {
        exit(heroine);
        return new DivingState(heroine);
    }
    return nullptr;
}

void JumpingState::update(Heroine &heroine) {
    LOG(INFO) << jump_time_;
    if (jump_time_ < 2.0) {
        jump_time_ += 0.1;
    } else {
        exit(heroine);
        heroine.state_ = new StandingState(heroine);
        // return new StandingState(heroine);
    }
}

void JumpingState::enter(Heroine &heroine) {
    LOG(INFO) << "enter " << name();
    heroine.setGraphics(Image::JUMP);
}

void JumpingState::exit(Heroine &heroine) {
    LOG(INFO) << "exit " << name();
}

std::string JumpingState::name() {
    return "JumpingState";
}

HeroineState *DivingState::handleInput(Heroine &heroine, Input input) {
    exit(heroine);
    return new DivingState(heroine);
}

void DivingState::update(Heroine &heroine) {

}

void DivingState::enter(Heroine &heroine) {
    LOG(INFO) << "enter " << name();
    heroine.setGraphics(Image::DIVE);
}

void DivingState::exit(Heroine &heroine) {
    LOG(INFO) << "exit " << name();
}

std::string DivingState::name() {
    return "DivingState";
}

HeroineState *MovingState::handleInput(Heroine &heroine, Input input) {
    if (input == Input::RELEASE_LEFT || input == Input::RELEASE_RIGHT) {
        exit(heroine);
        return new StandingState(heroine);
    } else if (input == Input::PRESS_DOWN) {
        exit(heroine);
        return new DuckingState(heroine);
    } else if (input == Input::PRESS_SPACE) {
        exit(heroine);
        return new DuckingState(heroine);
    }
    return nullptr;
}

void MovingState::update(Heroine &heroine) {

}

void MovingState::enter(Heroine &heroine) {

}

void MovingState::exit(Heroine &heroine) {

}

std::string MovingState::name() {
    return "MovingState";
}

HeroineState *EquipmentState::handleInput(Heroine &heroine, Input input) {
    if (input == Input::FIRE) {
        if (fire_interval_ <= 0.0) {
            fire_interval_ = 0.5;
            fire_ = true;
        }
    }
    return nullptr;
}

void EquipmentState::update(Heroine &heroine) {
    if (fire_) {
        fire_interval_ -= 0.1;
    }
    if (fire_interval_ <= 0.0) {
        fire_ = false;
    }
}

void EquipmentState::enter(Heroine &heroine) {
    LOG(INFO) << "enter " << name();
}

void EquipmentState::exit(Heroine &heroine) {
    LOG(INFO) << "exit " << name();
}

std::string EquipmentState::name() {
    if (fire_) {
        return "Fire";
    }
    return "EquipmentState";
}

void Heroine::handleInput(Input input) {
    HeroineState *state = state_->handleInput(*this, input);
    equipment_->handleInput(*this, input);
    if (state != nullptr) {
        state_ = state;
    }
}

void Heroine::update() {
    state_->update(*this);
    equipment_->update(*this);
}

Heroine::Heroine() {
    state_ = &HeroineState::standing;
    equipment_ = &HeroineState::equipment;
}

void Heroine::superBomb() {
    LOG(INFO) << "superBomb";
}

void Heroine::setGraphics(Image image) {
    LOG(INFO) << "setGraphics " << int(image);
}

void Heroine::init() {

}
