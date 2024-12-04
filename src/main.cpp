#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "Graph.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "TextureManager.h"

Game *game = nullptr;

// load graph of words connected to words used in their definition, weighted by the number of characters in that definition
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
                graph.connect(word, std::string(vec.begin(), vec.end()), def.length(), def);
                vec.clear();
                if(c == '\0') def = std::string(&line.data()[i + 1]);
            } else if(c != '(' && c != ')' && c != ',' && c != '.' && c != ';' && c != '\\' && c != '"')
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

void time_test(const std::string& name, std::function<void()> test) {
    auto start = std::chrono::high_resolution_clock::now();
    test();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << name << " took " << duration.count() << " millis" << std::endl;
}

int main() {
    Graph<std::string, std::string> graph;
    time_test("loadWordGraph", [&](){
        graph = loadWordGraph("../gen/opted.dict");
        // graph = loadWordGraph("../gen/wordset.dict");
    });

    time_test("bfs", [&]() {
        std::cout << std::endl << "BFS searching " << graph.getNodeCount() << " nodes, "
                << graph.getEdgeCount() << " edges..." << std::endl << std::endl;
        print_edges(graph, graph.bfs("dog", "approach"));
        std::cout << std::endl;
    });

    time_test("dfs", [&]() {
        std::cout << std::endl << "DFS searching " << graph.getNodeCount() << " nodes, "
                << graph.getEdgeCount() << " edges..." << std::endl << std::endl;
        print_edges(graph, graph.dfs("dog", "approach"));
        std::cout << std::endl;
    });

    time_test("dijkstras", [&]() {
        std::cout << std::endl << "Dijkstras shortest path searching " << graph.getNodeCount() << " nodes, "
                << graph.getEdgeCount() << " edges..." << std::endl << std::endl;
        print_edges(graph, graph.path(graph.dijkstras("dog"), "approach"));
        std::cout << std::endl;
    });

    game = new Game();
    game->init("Display",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1200,1200,false);


    while(game->running()) {

        game->handleEvents();
        game->update();
        game->render();
    }

    return 0;
}

