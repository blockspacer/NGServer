#include <thread>
#include <iostream>

#include <glog/logging.h>

#include "Heroine.h"

auto hero1 = Heroine();

void Update() {
    for (int i = 0; i < 1000; i++) {
        hero1.update();
        usleep(1000 * 100);
    }
}

void Input() {
    std::string key;
    while (getline(std::cin, key)) {
        if (key == "1") {}
        switch (key.c_str()[0]) {
            case '1':
                hero1.handleInput(Input::PRESS_DOWN);
                break;
            case '2':
                hero1.handleInput(Input::RELEASE_DOWN);
                break;
            case '3':
                hero1.handleInput(Input::PRESS_B);
                break;
            case '4':
                hero1.handleInput(Input::FIRE);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {

    std::thread t1(Update);
    std::thread t2(Input);

    t1.join();
    t2.join();

    return 0;
}

