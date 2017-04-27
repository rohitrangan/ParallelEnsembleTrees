#include "../include/adaboost.h"

AdaBoost::AdaBoost() : num_trees(10), max_depth(10), min_examples(10)
{
}

AdaBoost::AdaBoost(int num_trees, int max_depth, int min_examples)
{
    this->num_trees = num_trees;
    this->max_depth = max_depth;
    this->min_examples = min_examples;
}

void AdaBoost::train(Data& training)
{
    int dataset_size = training.get_dataset_size();
    std::vector<double> data_weights(dataset_size, (1.0 / dataset_size));
    std::vector<int> data_labels(training.get_labels());

    for(int i = 0; i < this->num_trees; ++i)
    {
        double eps_err = 0.0, alpha = 0.0, wt_sum = 0.0;
        DecisionTree dtree(this->max_depth, this->min_examples, false);
        dtree.train(training, data_weights);
        this->trees.push_back(dtree);
        std::vector<int> results = dtree.predict(training);
        for(int i = 0; i < dataset_size; ++i)
        {
            if(results[i] != data_labels[i])
                eps_err += data_weights[i];
        }
        if(eps_err - 0.01 < 0){
            eps_err = 0.01;
        }
        alpha = 0.5 * log((1.0 - eps_err) / eps_err);
        alphas.push_back(alpha);
        for(int i = 0; i < dataset_size; ++i)
        {
            data_weights[i] = data_weights[i] *
                exp(-(map_pred(results[i]) * map_pred(data_labels[i]) * alpha));
            wt_sum += data_weights[i];
        }
        for(int i = 0; i < dataset_size; ++i)
        {
            data_weights[i] = data_weights[i] / wt_sum;
        }
    }
}

std::vector<int> AdaBoost::predict(Data& testing)
{
    int dataset_size = testing.get_dataset_size();
    int num_trees = trees.size();
    std::vector<double> results_boostpred(dataset_size, 0.0);
    std::vector<int> results(dataset_size, 0);
    std::vector<int> tree_results;

    for(int i = 0; i < num_trees; ++i)
    {
        tree_results = trees[i].predict(testing);
        for(int j = 0; j < (int)tree_results.size(); ++j)
        {
            results_boostpred[j] += (alphas[i] * map_pred(tree_results[j]));
        }
    }
    for(int i = 0; i < (int)results.size(); ++i)
    {
        results[i] = (results_boostpred[i] >= 0.0) ? 1 : 0;
    }

    return results;
}

