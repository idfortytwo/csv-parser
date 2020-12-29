#include <iostream>
#include <CSVReader.hpp>

using namespace std;



int main() {
    string filename = "test.csv";

    CSVReader csv = CSVReader(filename);
    csv.print();
    csv.sort(2, false);

    return 0;
}
