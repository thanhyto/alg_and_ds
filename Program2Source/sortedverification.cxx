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
    vector<string> sampleName; //create a vector to store the samples
    //Loops over the JSON file to store the name of the sample
    for(auto it = data.begin(); it != data.end(); ++it){
        if(it.key() != "metadata") //Except the metadata key
            sampleName.push_back(it.key());
    }
    //Begin looping to check for the sorted array
    for(int i = 0; i < numSamp; i++){ //Takes in the name of the sample
        for(int j = 0; j < arrSize; j++){
            if( data[sampleName[i]][j] > data[sampleName[i]][j+1]){ //if A[j] < A[j+1]
                jsonOutput[sampleName[i]]["ConsecutiveInversions"][to_string(j)]={data[sampleName[i]][j],data[sampleName[i]][j+1]}; //Create "ConsecutiveInversions" key and assign the value of A[j] and A[j+1]
                inversion += 1; //Count the number of inversion in this array only
                jsonOutput[sampleName[i]]["sample"] = data[sampleName[i]];//Takes in the values of the array
            }
        }

        if(inversion != 0){
            invCount +=1; //count the total inversion in a file
            inversion = 0; //set the inversion of 1 array back to 0
        }
    }

    
    jsonOutput["metadata"]["samplesWithInversions"] = invCount; //assign "samplesWithInversions" with invCount after looping
    cout << jsonOutput << endl; //output the JSON
    return 1;
}