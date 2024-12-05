#pragma once

#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "Graph.h"

Graph<std::string, std::string> loadWordGraph(const std::string& file);

int recordTimeMillis(std::function<void()> func);
