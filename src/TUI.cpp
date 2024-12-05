#include "TUI.h"

#include <functional>
#include <iostream>
#include <string>

#include "Searching.h"
#include "Graph.h"

static std::pair<std::string, std::string> readFromAndTo() {
    std::string start, end;
    std::cout << "Start Word: ";
    std::getline(std::cin >> std::ws, start);
    std::cout << "End Word: ";
    std::getline(std::cin, end);
    return { start, end };
}

static void printEdges(const Graph<std::string, std::string>& graph, std::vector<GraphEdgeData<std::string>> edges) {
    for(const GraphEdgeData<std::string>& edge: edges) {
        std::cout << graph.getNode(edge.from) << ": " << edge.data << std::endl;
        std::cout << " |" << std::endl;
        std::cout << " V" << std::endl;
    }
    if(!edges.empty()) std::cout << graph.getNode(edges[edges.size() - 1].to) << std::endl;
}

static void bfs(Graph<std::string, std::string>& graph) {
    std::pair<std::string, std::string> input = readFromAndTo();
    std::cout << std::endl;
    printEdges(graph, graph.bfs(input.first, input.second));
}

static void dfs(Graph<std::string, std::string>& graph) {
    std::pair<std::string, std::string> input = readFromAndTo();
    std::cout << std::endl;
    printEdges(graph, graph.dfs(input.first, input.second));
}

static void sp(Graph<std::string, std::string>& graph) {
    std::pair<std::string, std::string> input = readFromAndTo();
    std::cout << std::endl;
    printEdges(graph, graph.path(graph.dijkstras(input.first), input.second));
}

void TUI::run() {
    std::cout << "Loading graph..." << std::endl;
    Graph<std::string, std::string> graph = loadWordGraph("../gen/opted.dict");
    std::cout << "Ready" << std::endl << std::endl;

    bool running = true;
    const std::pair<std::string, std::function<void(Graph<std::string, std::string>&)>> options[] = {
        { "[1] Breadth First Search (BFS)", bfs },
        { "[2] Depth First Search (DFS)", dfs },
        { "[3] Shortest Path (Dijkstra's)", sp },
        { "[4] Exit", [&](Graph<std::string, std::string>&) { running = false; } }
    };

    while(running) {
        for(const std::pair<std::string, std::function<void(Graph<std::string, std::string>&)>>& option: options)
            std::cout << option.first << std::endl;
        std::cout << "Choose option: ";

        int option;
        std::cin >> option;
        if(option < 1 || option > std::size(options)) std::cerr << "Invalid option!" << std::endl;
        else options[option - 1].second(graph);

        if(running) std::cout << std::endl;
    }
}
