#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;

int main(int argc, char** argv) {
    //File 1
    // Take the name of the JSON file as a command-line argument
    string fileName1 = argv[1];
    ifstream file1(fileName1);
    // Load the data from the JSON file to data
    nlohmann::json data1 = nlohmann::json::parse(file1);
    // Add the file name into metadata
    data1["metadata"]["name"] = fileName1 ;
    // Output the data
    cout << data1 << endl;
    // The JSON output after running the verification
    nlohmann::json jsonOutput;
    // Use the same metadata of the sample file
    jsonOutput["metadata"]["File1"] = data1["metadata"];
    //File 2
    string fileName2 = argv[2];
    ifstream file2(fileName2);
    // Load the data from the JSON file to data
    nlohmann::json data2 = nlohmann::json::parse(file2);
    // Add the file name into metadata
    data2["metadata"]["name"] = fileName2;
    //Output the data
    cout << data2 << endl;
    jsonOutput["metadata"]["File2"] = data2["metadata"];
    // Get the size of the array and the number of samples
    int arrSize1 = data1["metadata"]["arraySize"]; // size of the array in file 1
    int numSamp1 = data1["metadata"]["numSamples"]; // number of samples in file 1
    int arrSize2 = data2["metadata"]["arraySize"]; // size of the array in file 2
    int numSamp2 = data2["metadata"]["numSamples"]; // number of samples in file 2
    int conflict = 0; //number of conflicts in 1 sample
    int conCount = 0; //number of samples with conflicts
    vector<string> sampleName1; //create a vector to store the samples name from file 1
    vector<string> sampleName2; //create a vector to store the samples name from file 2
    // Loops over the JSON file1 to store the name of the sample
    for(auto it = data1.begin(); it != data1.end(); ++it){
        if(it.key() != "metadata") //Except the metadata key
            sampleName1.push_back(it.key());
    }
    //Loops over the JSON file2 to store the name of the sample 
    for(auto it = data2.begin(); it != data2.end(); ++it){
        if(it.key() != "metadata") //Except the metadata key
            sampleName2.push_back(it.key());
    }
    // //Begin looping to check for mismatches
    for(int i = 0; i < numSamp1; i++){ //Takes in the name of the sample
        for(int j = 0; j < arrSize1; j++){
            if(data1[sampleName1[i]][j] != data2[sampleName2[i]][j]){ //if A[j] != B[j]
                jsonOutput[sampleName1[i]][fileName2]= data2[sampleName2[i]]; //Store the name of the sample and the array of file 2
                conflict+=1; //increment the conflict count
                jsonOutput[sampleName1[i]]["Mismatches"][to_string(j)]={data1[sampleName1[i]][j], data2[sampleName2[i]][j]}; //Store the conflicting values from 2 arrays
                jsonOutput[sampleName2[i]][fileName1]= data1[sampleName1[i]]; //Store the array of file 1

            }
            
        }

        if(conflict != 0){
            conCount +=1; //count the total conflict in the file
            conflict = 0; //set the conflict of 1 array back to 0
        }
    }

    
    jsonOutput["metadata"]["samplesWithConflictingResults"] = conCount; //assign "samplesWithConflictingResults" with conCount after looping
    cout << jsonOutput << endl; //output the JSON
    // return 1;
}