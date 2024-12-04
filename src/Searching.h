#pragma once
#include "Graph.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <functional>


Graph<std::string, std::string> loadWordGraph(const std::string& file);

void print_edges(const Graph<std::string, std::string>& graph, std::vector<GraphEdgeData<std::string>> edges);

void test_bfs(const std::string& dict, const std::string& from, const std::string& to);

int time_test(const std::string& name, std::function<void()> test);