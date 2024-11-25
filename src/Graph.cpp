#include "Graph.h"

#include <iomanip>
using namespace std;

Graph::Graph() = default;

void Graph::insertEdge(string from, string to) {
    graph[from].push_back(to);
    in_out_degrees[from].second++;
    in_out_degrees[to].first++;

}



int Graph::getInDegree(const string& node) {
    return in_out_degrees[node].first;
}

int Graph::getOutDegree(const string& node) {
    return in_out_degrees[node].second;
}

int Graph::getV() {
    return in_out_degrees.size();
}

map<string, float> Graph::getRanks(int power) {
    auto const v = static_cast<float>(getV());
    for(int i=0;i<power;i++) {
        if(i==0) {
            for(const auto& j : in_out_degrees) {
                rank[j.first] = 1/v;
            }
        }
        else {
            map<string,float> temp_map;
            for(const auto& k : in_out_degrees) {
                //k is each node we are evaluating
                float new_rank=0;
                vector<string> incoming_vect = findIncoming(k.first);

                for(auto &l: incoming_vect) {
                    new_rank+= (rank[l])/(static_cast<float>(in_out_degrees[l].second));
                }
                temp_map[k.first] = new_rank;
            }
            rank = temp_map;
        }
    }

    for(const auto &i : rank) {
        cout<< i.first << " ";
        cout<<fixed<<showpoint;
        cout<<setprecision(2);
        cout<< i.second<<endl;
    }
    return rank;
}

vector<string> Graph::findIncoming(string node) {
    vector<string> count;
    for(const auto& i : graph) {
        for(const auto &j : i.second) {
            if(j == node) {
                count.push_back(i.first);
            }
        }
    }
    return count;
}
