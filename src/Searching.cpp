#include "Searching.h"

Graph<std::string, std::string> loadWordGraph(const std::string& file) {
    Graph<std::string, std::string> graph;
    std::ifstream ifs(file);

    std::string line;
    while(std::getline(ifs, line)) {
        std::string word(line.data());

        std::vector<char> vec;
        std::string def = std::string(&line.data()[word.length() + 1]);
        for(size_t i = word.length() + 1; i < line.length(); ++i) {
            char c = line[i];
            if(c == '\0' || c == ' ') {
                if(vec.size() == 0) continue;
                graph.connect(word, std::string(vec.begin(), vec.end()), def.size(), def);
                vec.clear();
                if(c == '\0') def = std::string(&line.data()[i + 1]);
            } else if(c != '(' && c != ')' && c != ',' && c != '.' && c != '\\' && c != '"')
                vec.push_back(std::tolower(c));
        }
    }

    ifs.close();
    return graph;
}

int recordTimeMillis(std::function<void()> func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}
