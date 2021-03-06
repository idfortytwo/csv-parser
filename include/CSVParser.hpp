#include <string>
#include <vector>

using namespace std;

#ifndef CSV_HPP
#define CSV_HPP

class CSVParser {
public:
    void readFile(const string& filename, char delimiter, bool hasHeader=false);
    void readHeaders(ifstream *file);

    vector<string> getRecord(int index);
    vector<string> getHeaders();
    vector<int> fieldTypes;
    int getFieldCount();

    enum Condition {
        filterLess,
        filterLessEqual,
        filterEqual,
        filterNotEqual,
        filterGreaterEqual,
        filterGreater
    };
    enum FieldType {
        typeString,
        typeNumber
    };

    void sort(int keyField, bool reverse=false);
    void filter(int field, int filter, const string& filterValue);
    void updateMaxFieldLengths();
    void print();
    void save(const string& outFilename, bool saveHeaders);

private:
    char delimiter;
    bool hasHeaders;

    vector<string> headers;
    vector<vector<string>> records;
    int fieldCount;

    bool dataChanged = true;
    vector<int> fieldMaxLengths;

    vector<vector<string>> fileToRecords(ifstream *file);
    vector<string> lineToFields(const string& line);

    template <typename T> bool filterConverted(T a, T b, int filter);

    static auto compareWrapperAsc(int &field, int &fieldType);
    static auto compareWrapperDesc(int &field, int &fieldType);
};

#endif  // CSV_HPP
