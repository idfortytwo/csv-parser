#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

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
        if (!line.empty()) {
            fields = lineToFields(line);
            fileRecords.push_back(fields);
        }
    }

    return fileRecords;
}


void CSVParser::readHeaders(ifstream *file) {
    string line;
    getline(*file, line);
    this->headers = lineToFields(line);
}

vector<string> CSVParser::getRecord(int index) {
    if (index < 0 && index >= this->fieldCount) {
        string errorMsg = "invalid record index: ";
        errorMsg += to_string(index);
        errorMsg += "\n";
        throw out_of_range(errorMsg);
    }

    return this->records[index];
}

vector<string> CSVParser::getHeaders() {
    return this->headers;
}

int CSVParser::getFieldCount() {
    if (this->records.empty())
        return 0;
    else
        return this->records[0].size();
}

void CSVParser::readFile(const string& filename, char fileDelimiter, bool fileHasHeader) {
    ifstream file;

    file.open(filename);
    if (!file) {
        string errorMsg = "failed to open file \"";
        errorMsg += filename;
        errorMsg += "\"\n";
        throw invalid_argument(errorMsg);
    }

    this->delimiter = fileDelimiter;
    this->hasHeaders = fileHasHeader;
    if (fileHasHeader)
        readHeaders(&file);

    this->records = fileToRecords(&file);

    file.close();

    this->fieldCount = getFieldCount();
    for (int i = 0; i < this->fieldCount; i++)
        this->fieldMaxLengths.push_back(0);
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
        std::sort(this->records.begin(), this->records.end(),
                  this->compareWrapperDesc(keyField, fieldType));
    else
        std::sort(this->records.begin(), this->records.end(),
                  this->compareWrapperAsc(keyField, fieldType));

    this->dataChanged = true;
}


void CSVParser::filter(int field, int filter, const string& filterValue) {
    vector<vector<string>> filtered;

    for (int i = 0; i < this->fieldCount; i++)
        this->fieldMaxLengths[i] = 0;

    int fieldType = this->fieldTypes[field];
    for (const auto& record : this->records) {
        string fieldValue = record[field];
        switch (fieldType) {
            case typeString:
                if (filterConverted(fieldValue, filterValue, filter))
                    filtered.push_back(record);
                break;

            case typeNumber:
                if (filterConverted(stod(fieldValue), stod(filterValue), filter))
                    filtered.push_back(record);
                break;

            case typeDate:
                if (filterConverted(fieldValue, filterValue, filter))
                    filtered.push_back(record);
                break;

            default:
                throw runtime_error("sorting compare function got unexpected field type\n");
        }
    }

    this->records = filtered;

    this->dataChanged = true;
}

template <typename T>
bool CSVParser::filterConverted(T a, T b, int filter) {
    switch (filter) {
        case filterLess:
            return a < b;

        case filterLessEqual:
            return a <= b;

        case filterEqual:
            return a == b;

        case filterNotEqual:
            return a != b;

        case filterGreaterEqual:
            return a >= b;

        case filterGreater:
            return a > b;

        default:
            string errorMsg = "unexpected filterConverted filter argument: \"";
            errorMsg += to_string(filter);
            errorMsg += "\"\n";
            throw runtime_error(errorMsg);
    }
}

void CSVParser::updateMaxFieldLengths() {
    if (this->hasHeaders) {
        for (int i = 0; i < this->fieldCount; i++) {
            int length = headers[i].length();
            if (this->fieldMaxLengths[i] < length)
                this->fieldMaxLengths[i] = length;
        }
    }

    for (const auto& record : this->records) {
        for (int i = 0; i < this->fieldCount; i++) {
            int length = record[i].length();
            if (this->fieldMaxLengths[i] < length)
                this->fieldMaxLengths[i] = length;
        }
    }

    this->dataChanged = false;
}

void CSVParser::print() {
    if (this->dataChanged)
        updateMaxFieldLengths();

    if (this->hasHeaders) {
        for (int i = 0; i < this->fieldCount; i++) {
            string header = this->headers[i];
            int maxLength = this->fieldMaxLengths[i];
            cout << left << setw(maxLength) << header << " ";
        }
        cout << endl;
    }

    for (const auto& record : this->records) {
        for (int i = 0; i < this->fieldCount; i++) {
            string field = record[i];
            int maxLength = this->fieldMaxLengths[i];
            cout << left << setw(maxLength) << field << " ";
        }
        cout << endl;
    }
}

void CSVParser::save(const string& outFilename, bool saveHeaders=false) {
    ofstream outFile;

    outFile.open(outFilename);
    if (outFile.is_open()) {
        if (saveHeaders && this->hasHeaders) {
            for (const auto& header : this->headers) {
                outFile << header << this->delimiter;
            }
            outFile.seekp(-1, ios_base::cur);
            outFile << endl;
        }

        for (const auto& record : this->records) {
            for (const auto &field : record) {
                outFile << field << this->delimiter;
            }
            outFile.seekp(-1, ios_base::cur);
            outFile << endl;
        }
        outFile << endl;

        cout << "saved to " << outFilename << endl;
        outFile.close();

    } else {
        string errorMsg = "unexpected error occured while writing to ";
        errorMsg += outFilename;
        throw runtime_error(errorMsg);
    }
}