#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;

int main(int argc, char** argv) {
    // Take the name of the JSON file as a command-line argument
    string fileName = argv[1];
    ifstream file(fileName);
    // Load the data from the JSON file to data
    nlohmann::json data = nlohmann::json::parse(file);
    // Add the file name into metadata
    data["metadata"]["fileName"] = fileName ;
    // Output the data
    cout << data << endl;
    // The JSON output after running the verification
    nlohmann::json jsonOutput;
    // Use the same metadata of the sample file
    jsonOutput["metadata"] = data["metadata"];
    // vector<int> sortSample = data["Sample1"].get<vector<int>>();
    // Get the size of the array and the number of samples
    int arrSize = data["metadata"]["arraySize"]; // size of the array
    int numSamp = data["metadata"]["numSamples"]; // number of samples
    int inversion = 0; //number of inversion in 1 sample
    int invCount = 0; //number of samples with inversions
    vector<string> sampleName;
    for(auto it = data.begin(); it != data.end(); ++it){
        if(it.key() != "metadata")
            cout << it.key() << it.value() << endl;
            sampleName.push_back(it.key());
    }
    for(int i = 0; i < numSamp; i++){
        for(int j = 0; j < arrSize; j++){
            if( data[sampleName[i]][j] > data[sampleName[i]][j+1]){
                jsonOutput[sampleName[i]]["ConsecutiveInversions"][to_string(j)]={data[sampleName[i]][j],data[sampleName[i]][j+1]};
                cout << data[sampleName[i]][j] << " " << data[sampleName[i]][j+1] << endl;
                inversion += 1;
            }
        }

        if(inversion != 0){
            vector<int> sortSample = data["Sample1"].get<vector<int>>();
            invCount +=1;
            inversion = 0;
        }
    }

    
    jsonOutput["metadata"]["samplesWithInversions"] = invCount;
    cout << jsonOutput << endl;
    return 1;
}