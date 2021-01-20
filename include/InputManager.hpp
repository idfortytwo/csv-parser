#include <string>

#include <CSVParser.hpp>

using namespace std;

#ifndef CSVMANAGER_HPP
#define CSVMANAGER_HPP

class InputManager {
    CSVParser parser;
    bool fileHasHeader=false;

    static void checkIfFileExists(const string& path);
    void printFieldOptions();
    int getFieldNumber();
    static bool yesNo();

public:
    void loadFile();
    void askTypes();
    void askNext();
    void show();
    void save();
    void sort();
    void filter();
};

#endif  // CSVMANAGER_HPP