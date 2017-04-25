#include "../include/random_forest.h"

Data RandomForest::sample_with_replacement(Data training)
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

    for(int i = 0; i < training.get_dataset_size(); ++i)
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

