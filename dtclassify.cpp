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

struct node{
	int identity;
	int attribute;
	float split_pos;
	int label;
	struct node* left;
	struct node* right;
};

void dtReconstruction(vector<node> preorder, vector<int> inorder, node* root){
  if(inorder.size() != 0){
    int index;
    vector<int>::iterator it;
    for(int i = 0; i < preorder.size(); i++){
      it = find(inorder.begin(), inorder.end(), preorder[i].identity);
      if(it != inorder.end()){
        index = preorder[i].identity;
        root->identity = preorder[i].identity;
        root->attribute = preorder[i].attribute;
        root->split_pos = preorder[i].split_pos;
        root->label = preorder[i].label;
        root->left = new node;
        root->right = new node;
        preorder.erase(preorder.begin() + i);
        break;
      }
    }

    vector<int> left, right;
    bool check = true;
    for(int i = 0; i < inorder.size(); i++){
      if(inorder[i] == index){
        check = false;
      }
      else if(check){
        left.push_back(inorder[i]);
      }
      else{
        right.push_back(inorder[i]);
      }
    }
    dtReconstruction(preorder, left, root->left);
    dtReconstruction(preorder, right, root->right);
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

int main(int argc, const char* argv[]) {
	string modelFile = argv[1];
  string testFile = argv[2];
  string prediction = argv[3];

  ifstream modelSet;
	ifstream dataSet;
	ofstream output;
  modelSet.open(modelFile);
	if (!modelSet) {
		cout << "Unable to open the " << modelFile << endl;
		exit(1);
	}
  dataSet.open(testFile);
  if (!dataSet) {
    cout << "Unable to open the " << testFile << endl;
    exit(1);
  }
	output.open(prediction);

  // Read test file.
  vector<vector<float>> testData;
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
    testData.push_back(temp);
  }
	cout<<testData[0].size()<<endl;
  cout<<testData.size()<<endl;
  dataSet.close();

  // Read model file
  vector<node> preorder;
  vector<int> inorder;
  bool pre = true;
  while(getline(modelSet, line)){
    node point;
    int count = 0, j = 0, comma = 0;
    if(line == "#Preorder Traversal"){
      pre = true;
    }
    else if(line == "#Inorder Traversal"){
      pre = false;
    }
    else if(pre){
      for(int i = 0; i < line.length(); i++){
        if(line[i] == ',' && comma == 0){
          point.identity = stoi(line.substr(j, count));
          j = i + 1;
          count = 0;
          comma++;
        }
        else if(line[i] == ',' && comma == 1){
          point.attribute = stoi(line.substr(j, count));
          j = i + 1;
          count = 0;
          comma++;
        }
        else if(line[i] == ',' && comma == 2){
          point.split_pos = stof(line.substr(j, count));
          j = i + 1;
          count = 0;
          comma++;
        }
        else{
          count++;
        }
      }
      point.label = stoi(line.substr(j, count));
      preorder.push_back(point);
    }
    else{
      inorder.push_back(stoi(line));
    }
  }

  // Reconstruct decision tree
  node* root = new node;
  dtReconstruction(preorder, inorder, root);

	// Testing
	for(int i = 0; i < testData.size(); i++){
		int predictedLabel = testing(root, testData[i]);
		output << testData[i][0] << ',' << predictedLabel << "\n";
	}

	output.close();
}
