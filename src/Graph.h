#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <map>
#include <set>
using namespace std;


class Graph {
private:
    map<string, vector<string>> graph;
    map<string, float> rank;
    map<string, pair<int,int>> in_out_degrees;

    int getInDegree(const string& node);
    int getOutDegree(const string& node);


public:
    Graph();
    void insertEdge(string from, string to);
    map<string, float>  getRanks(int power);
    vector<string> findIncoming(string node);
    int getV();
};



#endif //GRAPH_H
