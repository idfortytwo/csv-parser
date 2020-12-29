#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <CSVParser.hpp>

using namespace std;


vector<string> CSVParser::lineToFields(const string& line) {
    vector<string> fields;

    stringstream lineStream(line);
    while(lineStream) {
        string field;
        getline(lineStream, field, this->delimiter);
        fields.push_back(field);
    }
    fields.pop_back(); // ignore last line break "field"

    return fields;
}


vector<vector<string>> CSVParser::fileToRecords(ifstream *file) {
    vector<vector<string>> fileRecords;

    while (file) {
        string line;
        vector<string> fields;

        if (!getline(*file, line))
            break;

        fields = lineToFields(line);
        fileRecords.push_back(fields);
    }

    return fileRecords;
}


CSVParser::CSVParser(const string& filename, char delimiter) {
    this->inputFilename = filename;
    this->delimiter = delimiter;
}

void CSVParser::readHeaders(ifstream *file) {
    string line;
    getline(*file, line);
    this->headers = lineToFields(line);
}

bool CSVParser::hasHeaders() {
    return this->hasHeader;
}

vector<string> CSVParser::getHeaders() {
    return this->headers;
}

void CSVParser::readFile(bool hasHeaders) {
    ifstream file;
    file.open(this->inputFilename);

    if (hasHeaders) {
        readHeaders(&file);
        this->hasHeader = true;
    }

    this->fieldTypes.emplace_back(this->typeString);
    this->fieldTypes.emplace_back(this->typeNumber);
    this->fieldTypes.emplace_back(this->typeNumber);

    this->records = fileToRecords(&file);

    file.close();
}

auto CSVParser::compareWrapperAsc(int &field, int &fieldType) {
    return [&](vector<string> &a, vector<string> &b) {
        switch (fieldType) {
            case typeString:
                return a[field] < b[field];

            case typeNumber:
                return stod(a[field]) < stod(b[field]);

            case typeDate:
                return a[field] < b[field];

            default:
                throw runtime_error("sorting compare function got unexpected field type\n");
        }
    };
}

auto CSVParser::compareWrapperDesc(int &field, int &fieldType) {
    return [&](vector<string> &a, vector<string> &b) {
        switch (fieldType) {
            case typeString:
                return a[field] > b[field];

            case typeNumber:
                return stod(a[field]) > stod(b[field]);

            case typeDate:
                return a[field] > b[field];

            default:
                throw runtime_error("sorting compare function got unexpected field type\n");
        }
    };
}

void CSVParser::sort(int keyField, bool reverse) {
    int fieldType = this->fieldTypes[keyField];
    if (reverse)
        std::sort(this->records.begin(), this->records.end(), this->compareWrapperDesc(keyField, fieldType));
    else
        std::sort(this->records.begin(), this->records.end(), this->compareWrapperAsc(keyField, fieldType));
}


void CSVParser::filter(int field, Condition filter, const string& filterValue) {
    vector<vector<string>> filtered;

    int fieldType = this->fieldTypes[field];
    for (const auto& record : records) {
        switch (fieldType) {
            case typeString:
                if (filterConverted(record[field], filterValue, filter))
                    filtered.push_back(record);
                break;

            case typeNumber:
                if (filterConverted(stod(record[field]), stod(filterValue), filter))
                    filtered.push_back(record);
                break;

            case typeDate:
                if (filterConverted(record[field], filterValue, filter))
                    filtered.push_back(record);
                break;

            default:
                throw runtime_error("sorting compare function got unexpected field type\n");
        }
    }

//    print(filtered);
    this->records = filtered;
}

template <typename T>
bool CSVParser::filterConverted(T a, T b, Condition condition) {
    switch (condition) {
        case filterLess:
            return a < b;

        case filterLessEqual:
            return a <= b;

        case filterEqual:
            return a == b;

        case filterGreaterEqual:
            return a >= b;

        case filterGreater:
            return a > b;
    }
}

void CSVParser::print() {
    for (const auto& record : this->records) {
        for (const auto& field : record) {
            cout << field << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void CSVParser::print(const vector<vector<string>> &_records) {
    cout << _records.size() << endl;
    for (const auto& record : _records) {
        for (const auto& field : record) {
            cout << field << " ";
        }
        cout << endl;
    }
    cout << endl;
}