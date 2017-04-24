#include "../include/random_forest.h"

RandomForest::RandomForest() : num_trees(10), max_depth(10), min_examples(10),
                               subset_features(true)
{
}

RandomForest::RandomForest(int num_trees, int max_depth, int min_examples,
                           bool subset_features)
{
    this->num_trees = num_trees;
    this->max_depth = max_depth;
    this->min_examples = min_examples;
    this->subset_features = subset_features;
}

void Data::train(Data training)
{
    for(int i = 0; i < this->num_trees; ++i)
    {
        DecisionTree dtree(this->max_depth, this->min_examples,
                           this->subset_features);
        dtree.train(training);
        this->trees.push_back(dtree);
    }
}

std::vector<int> Data::predict(Data testing)
{
    int dataset_size = testing.get_dataset_size();
    std::vector<int> results(dataset_size, 0);
    std::vector<int> class_0_count(dataset_size, 0);
    std::vector<int> class_1_count(dataset_size, 0);

    for(int i = 0; i < num_trees; ++i)
    {
        std::vector<int> tree_results = this->trees[i].predict(testing);
        for(auto it = tree_results.begin(); it != tree_results.end(); ++it)
        {
            if(*it == 0)
                ++class_0_count[i];
            else
                ++class_1_count[i];
        }
    }

    for(int i = 0; i < dataset_size; ++i)
    {
        results[i] = (class_0_count[i] >= class_1_count[i]) ? 0 : 1;
    }

    return results;
}

