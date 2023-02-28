#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include "priorityqueue.h"
#include "json.hpp"
#include <set>
using namespace std;

int main(int argc, char** argv) {
    //open the file
    string fileName = argv[1];
    ifstream file(fileName);
    //Load the data into j
    nlohmann::json j = nlohmann::json::parse(file);

    // The JSON output after running the verification
    nlohmann::json jsonOutput;

    //Number of players
    int numPlayers = j["metadata"]["numPlayers"];
    //Create a set to keep track of players
    set<int> players;
    //Create a priority queue of win percentage
    PriorityQueue win_pq(numPlayers * (numPlayers - 1) / 2);

    double win;
    int playerOne;
    int playerTwo;
    
    for(int i = 0; i<(numPlayers * (numPlayers - 1) / 2); i++){
        //Calculate the winPercentage close to 50
        win = abs(j["teamStats"][i]["winPercentage"].get<double>()-50);
        //Get players
        playerOne=j["teamStats"][i]["playerOne"].get<int>();
        playerTwo=j["teamStats"][i]["playerTwo"].get<int>();
        //Insert KeyValuePair into priority queue
        KeyValuePair p;
        p.first = win;
        p.second.first = playerOne;
        p.second.second = playerTwo;
        win_pq.insert(p);

         
    }

    for(int k = 0; k < (numPlayers * (numPlayers - 1) / 2); k++){
        int remove1;
        int remove2;
        //KeyValuePair for the soon-to-be removed min
        KeyValuePair removedNode;
        removedNode = win_pq.min();
        //Store Value - playerOne, playerTwo
        remove1 = removedNode.second.first;
        remove2 = removedNode.second.second;
        //Check conditions
        if( (k==0) || ((players.find(remove1) == players.end()) &&(players.find(remove2) == players.end()))){
            jsonOutput["teams"][k]={remove1, remove2};
            players.insert(remove1);
            players.insert(remove2);
        }

        //Remove the min
        win_pq.removeMin();
    }
    //Format the output
    jsonOutput["teams"].erase(remove(jsonOutput["teams"].begin(), jsonOutput["teams"].end(), nullptr), jsonOutput["teams"].end());
    cout << jsonOutput << endl;

}