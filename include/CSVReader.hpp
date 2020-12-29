#include <string>
#include <vector>
#include "Records.hpp"

using namespace std;

#ifndef CSV_HPP
#define CSV_HPP

class CSVReader {
    vector<vector<string>> records;

    static vector<vector<string>> fileToRecords(ifstream *file);
    static vector<string> lineToFields(const string& line, char delimiter);

    static auto compareWrapperAsc(int &field);
    static auto compareWrapperDesc(int &field);

public:
    CSVReader(const string& filename);

    void sort(int field, bool asc=true);

    void print();
};

#endif  // CSV_HPP
