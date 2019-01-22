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

bool customComparison(pair<float, int> attr1, pair<float, int> attr2){
	return (attr1.first < attr2.first);
}

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

void preorderTraversal(node* point, ofstream& output){
	if(point != NULL){
		output << point->identity << ',';
		output << point->attribute << ',';
		output << point->split_pos << ',';
		output << point->label << "\n";
		preorderTraversal(point->left, output);
		preorderTraversal(point->right, output);
	}
}

void inorderTraversal(node* point, ofstream& output){
	if(point != NULL){
		inorderTraversal(point->left, output);
		output << point->identity << "\n";
		inorderTraversal(point->right, output);
	}
}

int main(int argc, const char* argv[]) {
	string trainFile = argv[1];
  int minfreq = atoi(argv[2]);
  string modelFile = argv[3];

	ifstream dataSet;
	ofstream output;
	dataSet.open(trainFile);
	if (!dataSet) {
		cout << "Unable to open the " << trainFile << endl;
		exit(1);
	}
	output.open(modelFile);

  // Read train file.
  vector<vector<float>> trainData;
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
    trainData.push_back(temp);
  }
	cout<<trainData[0].size()<<endl;
  cout<<trainData.size()<<endl;
  dataSet.close();

	// Classification
	node* root = new node;
	vector<int> usedAttribute;
	clock_t past, now;
	past = clock();
	buildDecisionTree(trainData, usedAttribute, minfreq, root);
	cout<<"Decision Tree has been built."<<endl;
	now = clock();
	cout << "Time: " << (float)(now - past) / CLOCKS_PER_SEC << endl;

	// Preorder Traversal
	output << "#Preorder Traversal\n";
	preorderTraversal(root, output);
	cout<<"Preorder Traversal completed."<<endl;
	// Inorder Traversal
	output << "#Inorder Traversal\n";
	inorderTraversal(root, output);
	cout<<"Inorder Traversal completed."<<endl;
	output.close();
}
