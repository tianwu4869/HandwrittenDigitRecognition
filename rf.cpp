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

int identity = 0;

struct node{
	int identity;
	int attribute;
	float split_pos;
	int label;
	struct node* left;
	struct node* right;
};

float computeGini(vector<vector<int>> countMatrix){
	vector<int> total(2, 0);
	for(int j = 0; j < countMatrix[0].size(); j++){
		for(int i = 0; i < countMatrix.size(); i++){
			total[j] += countMatrix[i][j];
		}
	}
	float gini = 0.0f;
	for(int j = 0; j < countMatrix[0].size(); j++){
		float temp = 1.0f;
		for(int i = 0; i < countMatrix.size(); i++){
			if(total[j] != 0){
				temp -= pow((float)countMatrix[i][j] / total[j], 2);
			}
		}
		gini += temp * ((float)total[j] / (total[0] + total[1]));
	}
	return gini;
}

vector<float> impurityOfAttribute(vector<vector<float>> trainData, map<float, vector<int>> sortedValue, int attribute){
	map<float, vector<int>>::iterator it = sortedValue.begin();
	vector<vector<int>> countMatrix(10, vector<int>(2, 0));
	// Fill out count matrix with left boundary case
	for(int j = 0; j < trainData.size(); j++){
		int label = trainData[j][0];
		if(trainData[j][attribute] <= it->first - 1.0f){
			countMatrix[label][0] += 1;
		}
		else{
			countMatrix[label][1] += 1;
		}
	}
	float lowestGini = computeGini(countMatrix);
	float pos = it->first - 1.0f;

	map<float, vector<int>>::iterator it0 = sortedValue.begin();
	it++;
	for(; it != sortedValue.end();){
		//cout<<"fsdffs"<<endl;
		float split_pos = (it0->first + it->first) / 2.0f;
		//cout<<sortedValue.size()<<endl;
		for(int j = 0; j < it0->second.size(); j++){
			countMatrix[it0->second[j]][0] += 1;
			countMatrix[it0->second[j]][1] -= 1;
		}
		// Compute Gini Index
		float gini = computeGini(countMatrix);
		if(gini < lowestGini){
			lowestGini = gini;
			pos = split_pos;
		}
		it++;
		it0++;
	}

	// Deal with right boundary case
	for(int j = 0; j < it0->second.size(); j++){
		countMatrix[it0->second[j]][0] += 1;
		countMatrix[it0->second[j]][1] -= 1;
	}
	float gini = computeGini(countMatrix);
	if(gini < lowestGini){
		lowestGini = gini;
		pos = it0->first + 1.0f;
	}

	// Return
	vector<float> impurity;
	impurity.push_back(lowestGini);
	impurity.push_back(pos);
	return impurity;
}

void buildDecisionTree(vector<vector<float>> trainData, vector<int> usedAttribute, int minfreq, node* point){
	// Check purity
	bool pure = true;
	vector<int> stats(10, 0);
	for(int k = 0; k < trainData.size(); k++){
		if(k != trainData.size() - 1 && trainData[k][0] != trainData[k + 1][0]){
			pure = false;
		}
		stats[(int)trainData[k][0]] += 1;
	}
	// Stop recursion or not
	if(!pure && trainData.size() >= minfreq && usedAttribute.size() != trainData[0].size() - 1){
		int bestAttribute;
		float minImpurity = 999.0f;
		float split_pos;
		vector<int>::iterator it;

		// Find the attribute that leads to purest classification
		for(int i = 1; i < trainData[0].size(); i++){
			// Check if this attribute has already been used
			it = find(usedAttribute.begin(), usedAttribute.end(), i);
			if(it == usedAttribute.end()){
				map<float, vector<int>> sortedValue;
				for(int j = 0; j < trainData.size(); j++){
					sortedValue[trainData[j][i]].push_back(trainData[j][0]);
				}
				//cout<<"Sorted value completed."<<endl;
				vector<float> impurity = impurityOfAttribute(trainData, sortedValue, i);
				//cout<<"impurity completed."<<endl;
				if(impurity[0] < minImpurity){
					bestAttribute = i;
					minImpurity = impurity[0];
					split_pos = impurity[1];
				}
			}
		}
		usedAttribute.push_back(bestAttribute);
		// Split data set based on the attribute picked above
		vector<vector<float>> smaller;
		vector<vector<float>> larger;
		for(int i = 0; i < trainData.size(); i++){
			if(trainData[i][bestAttribute] <= split_pos){
				smaller.push_back(trainData[i]);
			}
			else{
				larger.push_back(trainData[i]);
			}
		}
		// Recursion
		point->identity = identity;
		identity++;
		point->attribute = bestAttribute;
		point->split_pos = split_pos;
		point->label = -1;
		point->left = new node;
		point->right = new node;

		buildDecisionTree(smaller, usedAttribute, minfreq, point->left);
		buildDecisionTree(larger, usedAttribute, minfreq, point->right);
	}
	else{
		point->identity = identity;
		identity++;
		point->attribute = -1;
		point->split_pos = -1;
		point->left = NULL;
		point->right = NULL;
		int majority = -1;
		int label = -1;
		for(int i = 0; i < stats.size(); i++){
			if(stats[i] > majority){
				majority = stats[i];
				label = i;
			}
		}
		point->label = label;
	}
}

int testing(node* point, vector<float> instance){
	if(point->label == -1){
		int attribute = point->attribute;
		float split_pos = point->split_pos;
		if(instance[attribute] <= split_pos){
			return testing(point->left, instance);
		}
		else{
			return testing(point->right, instance);
		}
	}
	else{
		return point->label;
	}
}

vector<vector<float>> readDataFromFile(ifstream& dataSet){
  vector<vector<float>> data;
  string line;
  while (getline(dataSet, line)) {
    int j = 0, count = 0;
    vector<float> temp;
    float value;
    for(int i = 0; i < line.length(); i++){
      if(line[i] == ','){
        value = stof(line.substr(j, count));
        temp.push_back(value);
        j = i + 1;
        count = 0;
      }
      else{
        count++;
      }
    }
    value = stof(line.substr(j, count));
    temp.push_back(value);
    data.push_back(temp);
  }
  dataSet.close();
  return data;
}

int main(int argc, const char* argv[]) {
	string trainFile = argv[1];
  int minfreq = atoi(argv[2]);
  string testFile = argv[3];

	ifstream trainSet;
  ifstream testSet;
	trainSet.open(trainFile);
	if (!trainSet) {
		cout << "Unable to open the " << trainFile << endl;
		exit(1);
	}
  testSet.open(testFile);
	if (!testSet) {
		cout << "Unable to open the " << testFile << endl;
		exit(1);
	}

  // Read training file.
  vector<vector<float>> trainData = readDataFromFile(trainSet);
  // Read test file.
  vector<vector<float>> testData = readDataFromFile(testSet);

  // Generate 100 predictions
  srand(0);
  clock_t start, end;
  start = clock();
  for(int i = 1; i <= 100; i++){
    // Decide the name of output file
    string fileName = "pred";
    string index = to_string(i);
    for(int j = 0; j < (3 - index.length()); j++){
      fileName += '0';
    }
    fileName += index;
		ofstream output;
    output.open(fileName);

    // Sampling training set, 40%
    vector<vector<float>> chosen;
    int num_samples = 0.4f * trainData.size();
    vector<int> usedSamples;
    for(int j = 0; j < num_samples; j++){
      vector<int>::iterator it;
      int seq;
      do {
        seq = rand() % trainData.size();
        it = find(usedSamples.begin(), usedSamples.end(), seq);
      } while(it != usedSamples.end());
      chosen.push_back(trainData[seq]);
    }

    // Generate decision tree
    node* root = new node;
    vector<int> usedAttribute;
    clock_t past, now;
    past = clock();
    buildDecisionTree(chosen, usedAttribute, minfreq, root);

    // Testing
    for(int i = 0; i < testData.size(); i++){
      int predictedLabel = testing(root, testData[i]);
      output << testData[i][0] << ',' << predictedLabel << "\n";
    }
    cout<<fileName<<" generation completed."<<endl;
    now = clock();
    cout << "Time: " << (float)(now - past) / CLOCKS_PER_SEC << endl;
		output.close();
  }
  end = clock();
  cout << "Toal time cost: " << (float)(end - start) / CLOCKS_PER_SEC << endl;
}
