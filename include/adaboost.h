#ifndef __PARALLEL_ENSEMBLE_TREES_ADABOOST_H_
#define __PARALLEL_ENSEMBLE_TREES_ADABOOST_H_

#include <cmath>
#include <chrono>
#include <random>
#include <vector>
#include <utility>

#include "data.h"
#include "node.h"
#include "decision_tree.h"

#define map_pred(x) (((x) == 0) ? -1.0 : 1.0)

class AdaBoost
{
private:
    int num_trees;
    int max_depth;
    int min_examples;
    std::vector<DecisionTree> trees;
    std::vector<double> alphas;

public:
    AdaBoost();
    AdaBoost(int num_trees, int max_depth, int min_examples);

    void train(Data& training);
    std::vector<int> predict(Data& testing);
};

#endif /* __PARALLEL_ENSEMBLE_TREES_ADABOOST_H_ */

