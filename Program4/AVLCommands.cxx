#include "AVL.h"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

#include "json.hpp"

using namespace std;

int main(int argc, char** argv)
{
    //Open file
	ifstream file;
	file.open(argv[1]);
	nlohmann::json jsonObject;
	// Copy data in Json Object
	if (file.is_open()){
		file >> jsonObject;
	}
	    
	string op;

	AVL newAVLtree;

	for (auto itr = jsonObject.begin(); itr != (--jsonObject.end()); ++itr){ 
	    op = itr.key();
        int key = jsonObject[op]["key"];
		newAVLtree.recursiveInsert(key);
	}

      cout << newAVLtree.JSON() << endl;
}