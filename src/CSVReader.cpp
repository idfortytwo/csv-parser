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


void CSVReader::filter(int field, Operation operation, const string& value) {
    vector<vector<string>> filtered;

    cout << operation << endl;
    for (const auto& record : records) {
        switch (operation) {
            case less:
                cout << record[field] << " " << value << endl;
                if (record[field] < value)
                    filtered.push_back(record);
                break;

            case lessEqual:
                if (record[field] <= value)
                    filtered.push_back(record);
                break;

            case equal:
                if (record[field] == value)
                    filtered.push_back(record);
                break;

            case greater:
                if (record[field] >= value)
                    filtered.push_back(record);
                break;

            case greaterEqual:
                if (record[field] > value)
                    filtered.push_back(record);
                break;
        }
    }

    print(filtered);
}

void CSVReader::print() {
    for (const auto& record : this->records) {
        for (const auto& field : record) {
            cout << field << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void CSVReader::print(const vector<vector<string>> &_records) {
    cout << _records.size() << endl;
    for (const auto& record : _records) {
        for (const auto& field : record) {
            cout << field << " ";
        }
        cout << endl;
    }
    cout << endl;
}

