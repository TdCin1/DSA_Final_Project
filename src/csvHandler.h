#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;

void processCSV(const string& filePath, vector<vector<string>>& allData) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);

        string field;
        while (getline(ss, field, ',')) {
            row.push_back(field);
        }

        allData.push_back(row);
    }

    file.close();
}