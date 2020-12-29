#include <string>
#include <vector>

using namespace std;

#ifndef CSV_HPP
#define CSV_HPP

class CSVParser {
public:
    CSVParser(const string& filename, char delimiter);

    void readFile(bool hasHeader=false);
    void readHeaders(ifstream *file);

    bool hasHeaders();
    vector<string> getHeaders();

    void sort(int keyField, bool reverse=false);

    enum Condition {
        filterLess,
        filterLessEqual,
        filterEqual,
        filterGreaterEqual,
        filterGreater
    };

    enum FieldType {
        typeString,
        typeNumber,
        typeDate
    };

    void filter(int field, Condition operation, const string& filterValue);

    void print();
    static void print(const vector<vector<string>> &_records);

private:
    string inputFilename;
    char delimiter;

    bool hasHeader;
    vector<string> headers;
    vector<vector<string>> records;
    vector<int> fieldTypes;

    vector<vector<string>> fileToRecords(ifstream *file);
    vector<string> lineToFields(const string& line);

    template <typename T> bool filterConverted(T a, T b, Condition cond);

    static auto compareWrapperAsc(int &field, int &fieldType);
    static auto compareWrapperDesc(int &field, int &fieldType);
};

#endif  // CSV_HPP
