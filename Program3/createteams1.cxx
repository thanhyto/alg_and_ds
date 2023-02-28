
#include <iostream>
#include "json.hpp"
#include "priorityqueue.h"

using json = nlohmann::json;

// Struct to hold team statistics
struct TeamStats {
    int playerOne;
    int playerTwo;
    double winPercentage;
};

// Comparator for priority queue
struct WinPercentageCompare {
    bool operator()(const TeamStats& t1, const TeamStats& t2) const {
        return t1.winPercentage < t2.winPercentage;
    }
};

int main() {
    // Read input from stdin
    json input;
    std::cin >> input;

    // Extract number of players
    int numPlayers = input["metadata"]["numPlayers"].get<int>();

    // Create priority queue of team statistics
    PriorityQueue pq(numPlayers * (numPlayers - 1) / 2);

    // Fill priority queue with team statistics
    for (auto& teamStat : input["teamStats"]) {
        pq.insert({teamStat["playerOne"].get<int>(), teamStat["playerTwo"].get<int>(), teamStat["winPercentage"].get<double>()});
    }

    // Create array to keep track of assigned players
    std::vector<bool> assigned(numPlayers, false);

    // Create array to store final teams
    std::vector<std::vector<int>> teams(2);

    // Fill teams by selecting highest- and lowest-performing players not yet assigned
    for (auto& team : teams) {
        // Select highest-performing unassigned player
        int playerOne = -1;
        while (playerOne == -1) {
            auto topTeamStat = pq.min();
            pq.removeMin();
            if (!assigned[topTeamStat.playerOne] && !assigned[topTeamStat.playerTwo]) {
                playerOne = topTeamStat.playerOne;
                assigned[playerOne] = true;
                team.push_back(playerOne);
                team.push_back(topTeamStat.playerTwo);
            }
        }

        // Select lowest-performing unassigned player
        int playerTwo = -1;
        for (int i = 0; i < numPlayers; i++) {
            if (!assigned[i] && (playerTwo == -1 || input["teamStats"][i]["winPercentage"].get<double>() < input["teamStats"][playerTwo]["winPercentage"].get<double>())) {
                playerTwo = i;
            }
        }
        assigned[playerTwo] = true;
        team.push_back(playerTwo);
    }

    // Create output object and print to stdout
    json output;
    output["teams"] = teams;
    std::cout << output.dump(2) << std::endl;

    return 0;
}

