#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <CSVReader.hpp>

using namespace std;


vector<string> CSVReader::lineToFields(const string& line, char delimiter) {
    vector<string> fields;

    stringstream lineStream(line);
    while(lineStream) {
        string field;
        getline(lineStream, field, delimiter);
        fields.push_back(field);
    }
    fields.pop_back(); // ignore last line break "field"

    return fields;
}


vector<vector<string>> CSVReader::fileToRecords(ifstream *file) {
    vector<vector<string>> records;

    while (file) {
        string line;
        vector<string> fields;

        if (!getline(*file, line))
            break;

        fields = lineToFields(line, ',');
        records.push_back(fields);
    }

    return records;
}


CSVReader::CSVReader(const string& filename) {
    ifstream file;
    file.open(filename);
    records = fileToRecords(&file);
    file.close();
}

auto CSVReader::compareWrapperAsc(int &field) {
    return [&](vector<string> &a, vector<string> &b) {
        return a[field] < b[field];
    };
}

auto CSVReader::compareWrapperDesc(int &field) {
    return [&](vector<string> &a, vector<string> &b) {
        return a[field] > b[field];
    };
}

void CSVReader::sort(int field, bool ascending) {
    if (ascending)
        std::sort(this->records.begin(), this->records.end(), this->compareWrapperAsc(field));
    else
        std::sort(this->records.begin(), this->records.end(), this->compareWrapperDesc(field));
}

void CSVReader::print() {
    for (const auto& record : records) {
        for (const auto& field : record) {
            cout << field << " ";
        }
        cout << endl;
    }
    cout << endl;
}