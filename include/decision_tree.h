#ifndef __PARALLEL_ENSEMBLE_TREES_DECISION_TREE_H_
#define __PARALLEL_ENSEMBLE_TREES_DECISION_TREE_H_

#include <set>
#include <cmath>
#include <cfloat>
#include <chrono>
#include <random>
#include <vector>
#include <climits>
#include <iostream>

#include "data.h"
#include "node.h"

class DecisionTree
{
private:
    int max_depth;
    int min_examples;
    bool subset_features;
    Node *root;

    int get_best_feature(Node *node, std::vector< std::vector<int> >& features,
                         std::vector<int>& labels,
                         std::vector<double>& weights);
    int get_majority_label(Node* node, std::vector<int>& labels);
    int is_pure(Node* node, std::vector<int>& labels);
    Node* create_tree(std::vector< std::vector<int> >& features,
                      std::vector<int>& labels, std::vector<double>& weights,
                      int depth, std::set<int> data_ind,
                      std::set<int> feature_ind);
    int predict_one(Node* node, std::vector<int> feature);

public:
    DecisionTree();
    DecisionTree(int max_depth, int min_examples, bool subset_features);

    void train(Data& training);
    std::vector<int> predict(Data& testing);
};

#endif /* __PARALLEL_ENSEMBLE_TREES_DECISION_TREE_H_ */

