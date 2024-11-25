#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include<unordered_map>
#include<unordered_set>

using namespace std;


bool isBlank(const string& line) {
    return all_of(line.begin(), line.end(), [](unsigned char c) { return isspace(c); });
}

void processCSV(const string& filePath, vector<string>& allData) {
    //Open up the correct file with ifstream
    ifstream file(filePath);

    //Error checking
    if (!file.is_open()) {
        cerr << "Error: Could not open the file: " << filePath << endl;
        return;
    }

    //Adds all data into a 1d vector
    string line;
    while (getline(file, line)) {
        if(!isBlank(line)) {

            string row = "";
            stringstream ss(line);

            string field;
            while (getline(ss, field, ',')) {
                row += field;
            }

            allData.push_back(row);
        }
    }

    file.close();
}

unordered_map<string, unordered_set<string>> getDataMap(){
    //Has the correct path for our data
    string folderPath = "../Data/";
    vector<string> allData;

    //Processes each csv in our data folder
    try {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                cout << "Processing file: " << entry.path() << endl;
                processCSV(entry.path().string(), allData);
            }
        }

    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
    }


    unordered_map<string, unordered_set<string>> dataMap;


    for(const auto& data : allData){
        //Because of weird formatting must use this to find first letter in a string
        size_t index = string::npos;
        for (size_t i = 0; i < data.length(); ++i) {
            if (isalpha(data[i])) {
                index = i;
                break;  // Stop once the first letter is found
            }
        }

        size_t openingParen = data.find('(');
        string firstWord = data.substr(index, openingParen-1);

        //Trim extra spaces
        size_t end = firstWord.find_last_not_of(' ');

        //Remove all characters after the last non-space character
        if (end != string::npos) {
            firstWord.erase(end + 1);
        } else {
            // If the string is entirely made of spaces, clear it
            firstWord.clear();
        }

        // Find the first opening parenthesis '('
        if (openingParen != string::npos) {
            // Find the first closing parenthesis ')' after the opening parenthesis
            size_t closingParen = data.find(')', openingParen);

            string afterParenthesis;
            if (closingParen != string::npos && closingParen + 1 < data.length()) {
                // Extract everything after the closing parenthesis
                afterParenthesis = data.substr(closingParen + 1);
                // Trim leading spaces (optional)
                afterParenthesis.erase(0, afterParenthesis.find_first_not_of(' '));
            }

            //Now all of our definition is in afterParenthesis
            //We must separate each word and put it into a set
            vector<string> words;
            string currentWord;

            for (char c : data) {
                if (isalpha(c)) {
                    // If the character is a letter, add it to the current word
                    currentWord += c;
                } else {
                    // If a non-letter character is encountered, push the word (if any) to the vector
                    if (currentWord.size()>=3) {
                        words.push_back(currentWord);
                    }
                    currentWord.clear();  // Reset currentWord for the next word
                }
            }

            // If there is any word left in currentWord (after the loop ends), add it to the vector
            if (currentWord.size()>=3) {
                words.push_back(currentWord);
            }

            //Add all the data into the correct set
            for(const auto& i : words){
                dataMap[firstWord].insert(i);

            }
        }
    }

    return dataMap;
}