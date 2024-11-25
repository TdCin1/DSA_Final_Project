#include "csvHandler.h"
#include "Graph.h"

int main() {

    unordered_map<string, unordered_set<string>> allData = getDataMap();

    for(auto const& i : allData["Bat"]){
        cout<<i<<endl;
    }

    return 0;
}
