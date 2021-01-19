#include <iostream>
#include <InputManager.hpp>

using namespace std;


int main() {
    InputManager manager = InputManager();
    bool fileLoaded = false;

    while (!fileLoaded) {
        try {
            manager.loadFile();
            fileLoaded = true;
        } catch (exception &e) {
            cout << e.what() << endl;
        }
    }

    while (true) {
        try {
            manager.askNext();
        } catch (exception &e) {
            cout << e.what() << endl;
        }
    }
}