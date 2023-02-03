#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;

int main(int argc, char** argv) {
    ifstream file(argv[1]);
    nlohmann::json data = nlohmann::json::parse(file);
    cout << data << endl;

    
    return 1;
}