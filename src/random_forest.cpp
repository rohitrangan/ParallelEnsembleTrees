#include "../include/random_forest.h"

Data RandomForest::sample_with_replacement(Data& training)
{
    std::vector< std::vector<int> > features(training.get_features());
    std::vector<int> labels(training.get_labels());
    std::vector<double> weights(training.get_weights());

    std::vector< std::vector<int> > new_features;
    std::vector<int> new_labels;
    std::vector<double> new_weights;

    unsigned seed =
        std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> dist(0, training.get_dataset_size() - 1);

    for(int i = 0; i < (int)training.get_dataset_size(); ++i)
    {
        int data_select = dist(generator);
        new_features.push_back(features[data_select]);
        new_labels.push_back(labels[data_select]);
        new_weights.push_back(weights[data_select]);
    }

    return Data(new_features, new_labels, new_weights);
}

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

void RandomForest::train(Data& training)
{
    for(int i = 0; i < this->num_trees; ++i)
    {
        DecisionTree dtree(this->max_depth, this->min_examples,
                           this->subset_features);
        Data newly_sampled = sample_with_replacement(training);
        dtree.train(newly_sampled);
        this->trees.push_back(dtree);
    }
}

std::vector<int> RandomForest::predict(Data& testing)
{
    int dataset_size = testing.get_dataset_size();
    int num_trees = trees.size();
    std::vector<int> results(dataset_size, 0);
    std::vector<int> tree_results;

    for(int i = 0; i < num_trees; ++i)
    {
        tree_results = trees[i].predict(testing);
        for(int j = 0; j < (int)tree_results.size(); ++j)
        {
            results[j] += tree_results[j];
        }
    }
    for(int i = 0; i < (int)results.size(); ++i)
    {
        results[i] = (int)round((double)results[i] / (double)num_trees);
    }

    return results;
}

