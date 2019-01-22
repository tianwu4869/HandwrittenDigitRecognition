#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <time.h>
#include <math.h>
using namespace std;

int countDigits(int num){
  if(num / 10.0f < 1){
    return 1;
  }
  else{
    return countDigits(num / 10.0f) + 1;
  }
}

int main(int argc, const char* argv[]) {
  vector<pair<int, map<int, int>>> collections;
  // Read 100 prediction files.
  for(int k = 1; k <= 100; k++){
    // Construct the name of the file
    string prediction = "pred";
    string index = to_string(k);
    for(int m = 0; m < (3 - index.length()); m++){
      prediction += '0';
    }
    prediction += index;

    // Read files
    ifstream dataSet;
    dataSet.open(prediction);
  	if (!dataSet) {
  		cout << "Unable to open the " << prediction << endl;
  		exit(1);
  	}

    // Read prediction file
    int count = 0;
    while (1) {
      string t, p;
      int trueClass, predictedClass;
      getline(dataSet, t, ',');
      getline(dataSet, p, '\n');

      if(t.length() == 0){
        break;
      }

      trueClass = stoi(t);
      predictedClass = stoi(p);
      if(k == 1){
        map<int, int> stats;
        stats[predictedClass] += 1;
        collections.push_back(make_pair(trueClass, stats));
      }
      else{
        collections[count].second[predictedClass] += 1;
      }
      count++;
    }
    dataSet.close();
  }

  // Generate confusion matrix
  vector<vector<int>> confusionMatrix(10, vector<int>(10, 0));
  for(int i = 0; i < collections.size(); i++){
    map<int, int>::iterator it = collections[i].second.begin();
    int majority = 0;
    int maximum = 0;
    for(; it != collections[i].second.end(); it++){
      if(it->second > maximum){
        maximum = it->second;
        majority = it->first;
      }
    }
    confusionMatrix[collections[i].first][majority] += 1;
  }

  // Print out confusionMatrix
  cout<<"    ";
  for(int j = 0; j < confusionMatrix[0].size(); j++){
    cout << j << "    ";
  }
  cout<<endl;
  cout<<endl;
  int correctPrediction = 0, total = 0;
  for(int i = 0; i < confusionMatrix.size(); i++){
    cout << i << "   ";
    for(int j = 0; j < confusionMatrix[0].size(); j++){
      cout << confusionMatrix[i][j];
      int num_space = 5 - countDigits(confusionMatrix[i][j]);
      for(int k = 0; k < num_space; k++){
        cout << ' ';
      }
      total += confusionMatrix[i][j];
      if(i == j){
        correctPrediction += confusionMatrix[i][j];
      }
    }
    cout<<endl;
  }
  cout << "Accuracy: " << ((float) correctPrediction) / total << endl;
}
