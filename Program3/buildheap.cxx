#include <iostream>
#include <fstream>
#include "json.hpp"
#include "priorityqueue.h"

using namespace std;

int main(int argc, char** argv) {
    //open the file
    string fileName = argv[1];
    ifstream file(fileName);
    //Load the data into j
    nlohmann::json j = nlohmann::json::parse(file);
    nlohmann::json jsonOutput;
    int maxHeapSize = j["metadata"]["maxHeapSize"];
    // Create priority queue
    PriorityQueue pq(maxHeapSize);

    // Execute heap operations from JSON file
    int numOperations = j["metadata"]["numOperations"];
    vector<string> opName; //create a vector to store the samples
    //Loops over the JSON file to store the name of the sample
    for(auto it = j.begin(); it != j.end(); ++it){
        if(it.key() != "metadata") //Except the metadata key
            opName.push_back(it.key());
            
    }
    for (int i = 0; i < numOperations; i++) {
        std::string op = opName[i];
        if (j[op]["operation"] == "insert" && j[op]["key"] != NULL) {
            int key = j[op]["key"];
            pq.insert(key);
        } else if (j[op]["operation"] == "removeMin") {
            pq.removeMin();
        }
    }
    jsonOutput = nlohmann::json::parse(pq.JSON().dump(2));
    jsonOutput["metadata"]["numOperations"]= numOperations;
    cout << jsonOutput <<endl;

  return 0;
}

