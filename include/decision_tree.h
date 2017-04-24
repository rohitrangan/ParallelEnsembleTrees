#ifndef __PARALLEL_ENSEMBLE_TREES_DECISION_TREE_H_
#define __PARALLEL_ENSEMBLE_TREES_DECISION_TREE_H_

#include "data.h"
#include "node.h"

class DecisionTree
{
private:
    int max_depth;
    int min_examples;
    bool subset_features;
    Node *root;

    int get_best_feature(Node *node, Data training);

public:
    DecisionTree();
    DecisionTree(int max_depth, int min_examples, bool subset_features);

    void train(Data training);
    std::vector<int> predict(Data testing);
};

#endif /* __PARALLEL_ENSEMBLE_TREES_DECISION_TREE_H_ */