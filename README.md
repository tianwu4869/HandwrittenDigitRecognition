# HandwrittenDigitRecognition

Objective
The purpose of this project is to implement a decision tree-based classifiers for hand-written digit recognition. The project consists of multiple components that involve using different representations of the dataset, implementing two classification models, assessing the performance of each classifier with the different representations and different stopping criteria, and analyzing some of the models that were estimated. 

The daset
There two different feature representations for the data set, "rep1" and "rep2". "rep1" is the original MNIST dataset, "rep2" is generated by projecting the data points to the first 20 principal components of the original training dataset. For each representation, there is a training set and a test set; we train the classifiers on the training set and evaluate our model on the corresponding test set. The first column of each .csv file is the label and the remaining columns are the features.

Training
dtinduce <trainfile> <minfreq> <modelfile>
The training program will read in the training set and output the trained model. It will take three parameters as input: 
-<trainfile> is the training file.
-<minfreq> is the minimum number of data points in a node such that the node will keep splitting.
-<modelfile> is the file where we will write the learned decision tree.

Testing
dtclassify <modelfile> <testfile> <predictions>
The testing program will read in the model that was previously trained by the training program and classify the test set. It will take three parameters as input:  
-<modelfile> is the produced model file.
-<testfile> is the test file.
-<predictions> is the output file with the produced predictions. Note that, there are two columns in the predictions file: the first column contains the true labels of the data points and the second column contains the corresponding predicted labels. 

Evaluation
showconfmatrix <predictions>
This program takes the predictions as input, computes the confusion matrix and calculates the accuracy.