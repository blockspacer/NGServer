#include <glog/logging.h>

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

class Heroine {
public:
    void handleInput(Input input) {
        switch (state_) {
            case State::STANDING:
                if (input == Input::PRESS_B) {
                    state_ = State::JUMPING;
                    setGraphics(Image::JUMP);
                } else if (input == Input::PRESS_DOWN) {
                    state_ = State::DUCKING;
                    chargeTime_ = 0;
                    setGraphics(Image::DUCK);
                }
                break;
            case State::JUMPING:
                if (input == Input::PRESS_DOWN) {
                    state_ = State::DIVING;
                    setGraphics(Image::DIVE);
                }
                break;
            case State::DUCKING:
                if (input == Input::RELEASE_DOWN) {
                    state_ = State::STANDING;
                    setGraphics(Image::STAND);
                }
                break;
            default:
                break;
        }
    }

    void update() {
        if (state_ == State::DUCKING) {
            chargeTime_++;
            if (chargeTime_ > MAX_CHARGE) {
                superBomb();
            }
        }
    }

    void superBomb() {}

    void setGraphics(Image image) {}

private:
    State state_;
    float chargeTime_;
    static float const MAX_CHARGE;
};


int main(int argc, char *argv[]) {

    return 0;
}