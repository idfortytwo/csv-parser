#include <iostream>
#include <InputManager.hpp>

using namespace std;


int main() {
    InputManager manager = InputManager();
    manager.loadFile();

    while (true) {
        manager.askNext();
    }
}