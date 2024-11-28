#include "CSVHandler.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>

static bool isBlank(const std::string& line) {
    return std::all_of(line.begin(), line.end(), [](unsigned char c) { return isspace(c); });
}

static void processCSV(const std::string& filePath, std::vector<std::string>& allData) {
    // Open up the correct file with ifstream
    std::ifstream file(filePath);

    // Error checking
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file: " << filePath << std::endl;
        return;
    }

    // Adds all data into a 1d vector
    std::string line;
    while (getline(file, line)) {
        if(!isBlank(line)) {
            std::string row = "";
            std::stringstream ss(line);

            std::string field;
            while (getline(ss, field, ',')) {
                row += field;
            }

            allData.push_back(row);
        }
    }

    file.close();
}

static void lowercase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}

std::unordered_map<std::string, std::unordered_set<std::string>> getDataMap() {
    // Has the correct path for our data
    std::string folderPath = "../Data/";
    std::vector<std::string> allData;

    // Processes each csv in our data folder
    try {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                std::cout << "Processing file: " << entry.path() << std::endl;
                processCSV(entry.path().string(), allData);
            }
        }

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }


    std::unordered_map<std::string, std::unordered_set<std::string>> dataMap;


    for (const auto& data : allData) {
        // Because of weird formatting must use this to find first letter in a string
        size_t index = std::string::npos;
        for (size_t i = 0; i < data.length(); ++i) {
            if (isalpha(data[i])) {
                index = i;
                break;  // Stop once the first letter is found
            }
        }

        size_t openingParen = data.find('(');
        std::string firstWord = data.substr(index, openingParen-1);

        // Trim extra spaces
        size_t end = firstWord.find_last_not_of(' ');

        // Remove all characters after the last non-space character
        if (end != std::string::npos) {
            firstWord.erase(end + 1);
        } else {
            // If the string is entirely made of spaces, clear it
            firstWord.clear();
        }

        // Make lowercase
        lowercase(firstWord);

        // Find the first opening parenthesis '('
        if (openingParen != std::string::npos) {
            // Find the first closing parenthesis ')' after the opening parenthesis
            size_t closingParen = data.find(')', openingParen);

            std::string afterParenthesis;
            if (closingParen != std::string::npos && closingParen + 1 < data.length()) {
                // Extract everything after the closing parenthesis
                afterParenthesis = data.substr(closingParen + 1);
                // Trim leading spaces (optional)
                afterParenthesis.erase(0, afterParenthesis.find_first_not_of(' '));
            }

            // Now all of our definition is in afterParenthesis
            // We must separate each word and put it into a set
            std::vector<std::string> words;
            std::string currentWord;

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

            // Add all the data into the correct set
            for (std::string word : words){
                // Make word lowercase before inserting
                lowercase(word);
                dataMap[firstWord].insert(word);
            }
        }
    }

    return dataMap;
}
