Random-Forest
=============

Parallelization of the machine learning algorithm.

Well..The mathematical and logical stuff is exactly the grasping idea under the hood!

##Implementation Steps
* Decision Tree(Tree-based Regression, the CART Algorithm, GINI)
* Random Forest
* MPI

##Classes & Tech Details
The whole general logic of the program could be inspected within the main.cpp. 

1. DecisionTree - THE CORE PART OF THE PROGRAM. Use CART algorithm to implement the tree-based regression. I used Gini coefficient to decide the bestIndex and bestValue for splitting. Prepuning is implemented, while postpuning is neglected under the circumstances of random forest. 
	* Prepuning 1: If (originGINI - newGINI < tolS), it means splitting under the current (bestIndex, bestValue) has minor effect to the dataSet. So just set the current node as leaf.
	* Prepuning 2: If the number of samples within either of the splitted dataSets < tolN, set it to leaf, for the reason that small set of samples is not persuasive during classification.
2. FileReader - Be responsible for reading the data in the .csv file, including both the train and the test data. I use 4 threads to read the data simultaneously using file input/output stream. So in the directory, I split the train.csv into train1.csv ~ train4.csv, each of which include 5000. The science is the same for the test.csv, seperated into 4 shares, each of which containing 10000 samples.
3. Tester - Run each sample to the tree and return a classification result.
4. FileWriter - In charge of writing the results to out.csv after training and testing. 


##Keynotes: Some Intriguing Markdowns
###1.Optimization measures

* Use STL set to avoid duplicated value split operation.
* Use non-recursive method (Heap) to build up the tree.
* Parallelization operation, multithreading when operating prediction in both one single tree and the whole forest.

###2.Mathematical ideas
* When reducing the number of training features from p to √p, the number of rows for training should also be shrank to 1/2 of the original count. Avoiding high variances in the leaf node of the tree.

###3.Adjustment
* Choosing the MODE but not MEAN when creating leaf node. MEAN appears to be meaningless in this specific case. 