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
                graph.connect(word, std::string(vec.begin(), vec.end()), def);
                vec.clear();
                if(c == '\0') def = std::string(&line.data()[i + 1]);
            } else if(c != '(' && c != ')' && c != ',' && c != '.' && c != '\\' && c != '"')
                vec.push_back(std::tolower(c));
        }
    }

    ifs.close();
    return graph;
}

void print_edges(const Graph<std::string, std::string>& graph, std::vector<GraphEdgeData<std::string>> edges) {
    for(GraphEdgeData<std::string> edge: edges)
        std::cout << graph.getNode(edge.from) << ": " << edge.data << std::endl;
    if(edges.size() > 0) std::cout << graph.getNode(edges[edges.size() - 1].to) << std::endl;
}

void test_bfs(const std::string& dict, const std::string& from, const std::string& to) {
    Graph<std::string, std::string> graph = loadWordGraph(dict);
}

int time_test(const std::string& name, std::function<void()> test) {
    auto start = std::chrono::high_resolution_clock::now();
    test();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}