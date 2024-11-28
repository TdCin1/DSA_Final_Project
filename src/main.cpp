#include <unordered_set>
#include <iostream>
#include <vector>
#include <queue>
#include <set>

#include "CSVHandler.h"
#include "Graph.h"

// could move to Graph.h
template <typename T>
std::vector<GraphNodePtr<T>> bfs(Graph<T>& graph, T from, T to) {
    std::queue<std::vector<GraphNodePtr<T>>> q;
    std::set<std::vector<GraphNodePtr<T>>> v;

    GraphNodePtr<T> f = graph.find(from);
    GraphNodePtr<T> t = graph.find(to);
    if(!f || !t) return {};
    q.push({f});

    while(!q.empty()) {
        std::vector<GraphNodePtr<T>> path = q.front();
        q.pop();

        for(GraphNodePtr<T> conn: path[path.size() - 1]->getConnections()) {
            std::vector<GraphNodePtr<T>> next(path);
            next.push_back(conn);
            if(v.count(next) > 0) continue;

            if(conn == t) return next;
            q.push(next);
            v.insert(next);
        }
    }

    return {};
}

// TODO: alt shows up in graph, is an error of definition parsing
int main() {
    Graph<std::string> graph;

    std::unordered_map<std::string, std::unordered_set<std::string>> data = getDataMap();
    for(std::unordered_map<std::string, std::unordered_set<std::string>>::iterator it = data.begin();
            it != data.end(); ++it) {
        for(std::string word: it->second)
            graph.connect(it->first, word);
    }

    // find path from dog to cat and print
    for(GraphNodePtr<std::string> node: bfs<std::string>(graph, "dog", "cat"))
        std::cout << node->getValue() << std::endl;

    return 0;
}
