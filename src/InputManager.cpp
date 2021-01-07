#include <string>
#include <iostream>

#include <InputManager.hpp>

using namespace std;

void InputManager::loadFile() {
    string input;

    string filename;
    char delimiter;
    bool hasHeader;

    cout << "provide filename:" << endl;
    cout << ">";
    cin >> filename;

    cout << "does it have headers?" << endl;
    hasHeader = yesNo();

    cout << "provide delimiter character:" << endl;
    cout << ">";
    cin >> input;

    if (input.length() != 1) {
        string errorMsg = "recieved: \"";
        errorMsg += input;
        errorMsg += "\"\nexpected: single character\n";
        throw invalid_argument(errorMsg);
    } else {
        delimiter = input[0];
    }

    this->fileHasHeader = hasHeader;
    this->parser.readFile(filename, delimiter, hasHeader);

    askTypes();
}

void InputManager::askTypes() {
    string input;
    vector<string> values;

    cout << "data types:" << endl;
    cout << "string" << endl;
    cout << "number" << endl;
    cout << "date" << endl << endl;

    if (this->fileHasHeader)
        values = this->parser.getHeaders();
    else
        values = this->parser.getRecord(0);

    for (int i = 0; i < this->parser.getFieldCount(); i++) {
        int type;

        cout << "choose type for field ";
        if (this->fileHasHeader) {
            cout << '"' << values[i] << '"' << endl;
        } else {
            cout << "number " << i+1 << " (\"" << values[i] << "\")" << endl;
        }

        cout << ">";
        cin >> input;

        if (input == "string") {
            type = this->parser.typeString;
        } else if (input == "number") {
            type = this->parser.typeNumber;
        } else if (input == "date") {
            type = this->parser.typeDate;
        } else {
            string errorMsg = "recieved: \"";
            errorMsg += input;
            errorMsg += "\"\nexpected: single character\n";
            throw invalid_argument(errorMsg);
        }

        this->parser.fieldTypes.push_back(type);
    }
}

void InputManager::askNext() {
    string input;

    cout << endl;
    cout << "show   - show current records" << endl;
    cout << "save   - save to csv file" << endl;
    cout << "sort   - sort by field" << endl;
    cout << "filter - filter by field value" << endl;
    cout << "exit   - exit program" << endl;

    cout << ">";
    cin >> input;

    if (input == "show") {
        show();
    } else if (input == "save") {
        save();
    } else if (input == "sort") {
        sort();
    } else if (input == "filter") {
        filter();
    } else if (input == "exit") {
        exit(0);
    } else {
        string errorMsg = "recieved: \"";
        errorMsg += input;
        errorMsg += "\"\nexpected: \"save\", \"save\", \"sort\" or \"filter\"\n";
        throw invalid_argument(errorMsg);
    }
}

void InputManager::show() {
    this->parser.print();
}

void InputManager::save() {
    string newFilename;
    bool saveHeaders = false;

    cout << "new file name:" << endl;
    cout << ">";
    cin >> newFilename;

    if (fileHasHeader) {
        cout << "save headers?" << endl;
        saveHeaders = yesNo();
    }

    this->parser.save(newFilename, saveHeaders);
}

void InputManager::printFieldOptions() {
    if (this->fileHasHeader) {
        cout << "choose field:" << endl;
        for (int i = 0; i < this->parser.getFieldCount(); i++) {
            cout << i+1 << " - " << this->parser.getHeaders()[i] << endl;
        }
    } else {
        cout << "choose field number:" << endl;
        for (int i = 0; i < this->parser.getFieldCount(); i++) {
            cout << i+1 << endl;
        }
    }
}

int InputManager::getFieldNumber() {
    string input;
    int fieldNumber;

    printFieldOptions();
    cout << ">";
    cin >> input;

    try {
        fieldNumber = stoi(input);
    } catch (...) {
        string errorMsg = "recieved: \"";
        errorMsg += input;
        errorMsg += "\"\nexpected: field number";
        throw invalid_argument(errorMsg);
    }
    if (fieldNumber <= 0 || fieldNumber > this->parser.getFieldCount()) {
        string errorMsg = "recieved: \"";
        errorMsg += input;
        errorMsg += "\"\nexpected: field number";
        throw invalid_argument(errorMsg);
    }

    return fieldNumber;
}

bool InputManager::yesNo() {
    string input;

    cout << "y/n" << endl;
    cout << ">";
    cin >> input;

    if (input == "y") {
        return true;
    } else if (input == "n") {
        return false;
    } else {
        string errorMsg = "recieved: \"";
        errorMsg += input;
        errorMsg += "\"\nexpected: \"y\" or \"n\"\n";
        throw invalid_argument(errorMsg);
    }
}

void InputManager::sort() {
    string input;
    int keyField;
    bool reverse;

    keyField = getFieldNumber();

    cout << "revert order?" << endl;
    reverse = yesNo();

    this->parser.sort(keyField-1, reverse);
}

void InputManager::filter() {
    string input;

    int keyField;
    int filter;
    string filterValue;

    keyField = getFieldNumber();

    cout << "choose comparison filter:" << endl;
    cout << "<  - less than" << endl;
    cout << "<= - less than or equal" << endl;
    cout << "=  - equal to" << endl;
    cout << "!= - not equal to" << endl;
    cout << ">= - greater than or equal" << endl;
    cout << ">  - greater than" << endl;
    cout << ">";
    cin >> input;

    if (input == "<") {
        filter = this->parser.filterLess;
    } else if (input == "<=") {
        filter = this->parser.filterLessEqual;
    } else if (input == "=") {
        filter = this->parser.filterEqual;
    } else if (input == "!=") {
        filter = this->parser.filterNotEqual;
    } else if (input == ">=") {
        filter = this->parser.filterGreaterEqual;
    } else if (input == ">") {
        filter = this->parser.filterGreater;
    } else {
        string errorMsg = "recieved: \"";
        errorMsg += input;
        errorMsg += "\"\nexpected: \"<\", \"<=\", \"=\", \">\" or \">=\"\n";
        throw invalid_argument(errorMsg);
    }

    cout << "provide filter value" << endl;
    cout << ">";
    cin >> filterValue;

    this->parser.filter(keyField-1, filter, filterValue);
}
