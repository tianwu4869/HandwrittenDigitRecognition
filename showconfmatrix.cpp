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
  string prediction = argv[1];

  ifstream dataSet;
  dataSet.open(prediction);
	if (!dataSet) {
		cout << "Unable to open the " << prediction << endl;
		exit(1);
	}

  // Read prediction file
  vector<vector<int>> confusionMatrix(10, vector<int>(10, 0));
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

    confusionMatrix[trueClass][predictedClass] += 1;
  }
  dataSet.close();

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
