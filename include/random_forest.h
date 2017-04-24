#ifndef __PARALLEL_ENSEMBLE_TREES_RANDOM_FOREST_H_
#define __PARALLEL_ENSEMBLE_TREES_RANDOM_FOREST_H_

#include <vector>
#include <utility>

#include "data.h"
#include "node.h"
#include "decision_tree.h"

class RandomForest
{
private:
    int num_trees;
    int max_depth;
    int min_examples;
    bool subset_features;
    std::vector<DecisionTree> trees;

public:
    RandomForest();
    RandomForest(int num_trees, int max_depth, int min_examples,
                 bool subset_features);

    void train(Data training);
    std::vector<int> predict(Data testing);
};

#endif /* __PARALLEL_ENSEMBLE_TREES_RANDOM_FOREST_H_ */

