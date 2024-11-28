#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <set>

template <typename T>
class GraphNode;

template <typename T>
using GraphNodePtr = std::shared_ptr<GraphNode<T>>;

template <typename T>
class GraphNode {
private:
    std::set<GraphNodePtr<T>> connections; // adjacency list, holds all nodes that this node is connected to
    T value;

public:
    GraphNode(T value) {
        this->value = value;
    }

    std::set<GraphNodePtr<T>>& getConnections() {
        return connections;
    }

    T getValue() const {
        return value;
    }
};

template <typename T>
class Graph {
private:
    std::map<T, int> indices; // holds indices of vector 'nodes' for fast searching, and ordered traversal based on type T
    std::vector<GraphNodePtr<T>> nodes; // holds all nodes of the graph, with each node having its adjacency list in its class
    int size = 0;

public:
    GraphNodePtr<T> find(T value) {
        typename std::map<T, int>::iterator it = indices.find(value);
        if(it != indices.end()) return nodes[it->second];
        return nullptr;
    }

    // try to find node by value, if doesn't exist then add to graph
    GraphNodePtr<T> findAdd(T value) {
        GraphNodePtr<T> node = find(value);
        if(!node) {
            node = std::make_shared<GraphNode<T>>(value);
            nodes.push_back(node);
            indices[value] = size++;
        }
        return node;
    }

    void connect(GraphNodePtr<T> from, GraphNodePtr<T> to) {
        from->getConnections().insert(to);
    }

    void connect(T from, T to) {
        GraphNodePtr<T> f = findAdd(from);
        GraphNodePtr<T> t = findAdd(to);
        return connect(f, t);
    }

    // prints all nodes with their adjacent nodes listed
    void print() {
        for(GraphNodePtr<T> node: nodes) {
            std::cout << node->getValue() << ": ";
            for(GraphNodePtr<T> connected: node->getConnections())
                std::cout << connected->getValue() << " ";
            std::cout << std::endl;
        }
    }

    const std::vector<GraphNodePtr<T>>& getNodes() const {
        return nodes;
    }

    int getSize() const {
        return size;
    }
};
