#include "csvHandler.h"
#include "Graph.h"




int main() {
    string folderPath = "../Data/";
    vector<vector<string>> allData;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                cout << "Processing file: " << entry.path() << endl;
                processCSV(entry.path().string(), allData);
            }
        }


        for (const auto& row : allData) {
            for (const auto& field : row) {
                cout << field << " ";
            }
            cout << endl;
        }
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
    }

    return 0;
}
