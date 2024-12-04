#pragma once

#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>
#include <memory>
#include <queue>
#include <stack>

typedef int GraphNode;
typedef int GraphEdge;

template <typename R>
struct GraphEdgeData {
    GraphNode from;
    GraphNode to;
    int weight;
    R data;
};

// T is node data, R is graph edge data
template <typename T, typename R>
class Graph { // weighted directed graph with edge and node data
    private:
        std::vector<std::unordered_set<GraphEdge>> adj; // each node has a vector of edges represented as indices
        std::unordered_map<T, GraphNode> indices; // map of node by value to index
        std::vector<GraphEdgeData<R>> edges;
        std::vector<T> values;
        GraphEdge edgeCount = 0;
        GraphNode nodeCount = 0;

    public:
        // returns node -1 if value not in graph
        GraphNode find(T value) {
            typename std::unordered_map<T, GraphNode>::iterator it = indices.find(value);
            if(it != indices.end()) return it->second;
            return -1;
        }

        // try to find node by value, if doesn't exist then add to graph
        GraphNode findAdd(T value) {
            GraphNode node = find(value);
            if(node == -1) {
                adj.push_back({});
                node = indices[value] = nodeCount++;
                values.push_back(value);
            }
            return node;
        }

        T getNode(GraphNode node) const {
            return values[node];
        }

        GraphEdgeData<R> getEdgeData(GraphEdge edge) const {
            return edges[edge];
        }

        std::unordered_set<GraphEdge> getEdges(GraphNode node) const {
            return adj[node];
        }

        void connect(GraphNode from, GraphNode to, int weight, R data) {
            edges.push_back({from, to, weight, data});
            adj[from].insert(edgeCount++);
        }

        void connect(T from, T to, int weight, R data) {
            GraphNode f = findAdd(from);
            GraphNode t = findAdd(to);
            return connect(f, t, weight, data);
        }

        // returns empty vector when node not found
        std::vector<GraphEdgeData<R>> bfs(GraphNode from, GraphNode to) {
            std::queue<GraphNode> q;
            std::vector<bool> v(nodeCount, false); // visited nodes
            std::vector<GraphEdge> p(nodeCount, -1); // predecessors, by edge

            q.push(from);
            v[from] = true;

            while(!q.empty()) {
                GraphNode curr = q.front();
                q.pop();

                for(GraphEdge e: adj[curr]) {
                    GraphEdgeData<R> edge = edges[e];
                    if(v[edge.to]) continue;
                    v[edge.to] = true;
                    q.push(edge.to);
                    p[edge.to] = e;

                    if(edge.to == to) return path(p, to);
                }
            }
            return {};
        }

        std::vector<GraphEdgeData<R>> bfs(T from, T to) {
            GraphNode f = find(from);
            GraphNode t = find(to);
            if(f == -1 || t == -1) return {};
            return bfs(f, t);
        }

        std::vector<GraphEdgeData<R>> dfs(GraphNode from, GraphNode to) {
            std::stack<GraphNode> s;
            std::vector<bool> v(nodeCount, false); // visited nodes
            std::vector<GraphEdge> p(nodeCount, -1); // predecessors, by edge

            s.push(from);
            v[from] = true;

            while(!s.empty()) {
                GraphNode curr = s.top();
                s.pop();

                for(GraphEdge e: adj[curr]) {
                    GraphEdgeData<R> edge = edges[e];
                    if(v[edge.to]) continue;
                    v[edge.to] = true;
                    s.push(edge.to);
                    p[edge.to] = e;

                    if(edge.to == to) return path(p, to);
                }
            }
            return {};
        }

        std::vector<GraphEdgeData<R>> dfs(T from, T to) {
            GraphNode f = find(from);
            GraphNode t = find(to);
            if(f == -1 || t == -1) return {};
            return dfs(f, t);
        }

        std::vector<GraphEdge> dijkstras(GraphNode from) {
            std::priority_queue<std::pair<int, GraphNode>, std::vector<std::pair<int, GraphNode>>,
                    std::greater<std::pair<int, GraphNode>>> pq;
            std::vector<GraphEdge> p(nodeCount, -1);
            std::vector<int> d(nodeCount, INT_MAX);

            pq.push({0, from});
            d[from] = 0;

            while(!pq.empty()) {
                GraphNode curr = pq.top().second;
                pq.pop();

                for(GraphEdge e: adj[curr]) {
                    GraphEdgeData<R> edge = getEdgeData(e);
                    if(d[edge.to] > d[curr] + edge.weight) {
                        d[edge.to] = d[curr] + edge.weight;
                        p[edge.to] = e;
                        pq.push({d[edge.to], edge.to});
                    }
                }
            }

            return p;
        }

        std::vector<GraphEdge> dijkstras(T from) {
            GraphNode f = find(from);
            if(f == -1) return {};
            return dijkstras(f);
        }

        std::vector<GraphEdgeData<R>> path(std::vector<GraphEdge> p, GraphNode to) {
            std::vector<GraphEdgeData<R>> path;
            for(GraphEdge e = p[to]; e != -1;) {
                GraphEdgeData<R> edge = getEdgeData(e);
                path.push_back(edge);
                e = p[edge.from];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        std::vector<GraphEdgeData<R>> path(std::vector<GraphEdge> p, T to) {
            GraphNode t = find(to);
            if(t == -1) return {};
            return path(p, t);
        }

        int getNodeCount() const {
            return nodeCount;
        }

        int getEdgeCount() const {
            return edgeCount;
        }
};
