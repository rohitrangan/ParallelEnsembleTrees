#include <mpi.h>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <iostream>

#include "../include/data.h"
#include "../include/node.h"
#include "../include/decision_tree.h"

Data sample_with_replacement(Data training)
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

std::vector<DecisionTree> train_rf(Data& training, int num_trees, int max_depth,
                                   int min_examples, bool subset_features)
{
    std::vector<DecisionTree> trees;
    for(int i = 0; i < num_trees; ++i)
    {
        DecisionTree dtree(max_depth, min_examples,
                           subset_features);
        Data newly_sampled = sample_with_replacement(training);
        dtree.train(newly_sampled);
        trees.push_back(dtree);
    }
    return trees;
}

std::vector<int> predict_rf(Data& testing, std::vector<DecisionTree> trees)
{
    int dataset_size = testing.get_dataset_size();
    int num_trees = trees.size();
    std::vector<int> results(dataset_size, 0);
    std::vector<int> class_0_count(dataset_size, 0);
    std::vector<int> class_1_count(dataset_size, 0);

    for(int i = 0; i < num_trees; ++i)
    {
        std::vector<int> tree_results = trees[i].predict(testing);
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

int main(int argc, char* argv[])
{
    int rank, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    int all_trees = std::stoi(argv[5]);
    int num_trees_proc = all_trees / nproc;
    int max_depth = 10;
    int min_examples = 10;
    bool subset_features = true;

    Data training(argv[1], argv[2]);
    Data testing(argv[3], argv[4]);
    std::vector<DecisionTree> trees =
        train_rf(training, num_trees_proc, max_depth, min_examples, subset_features);
    std::vector<int> res = predict_rf(testing, trees);
    int* recv_res = nullptr;
    if(rank == 0)
    {
        recv_res = new int(res.size());
    }

    MPI_Reduce(&res[0], recv_res, (int)res.size(), MPI_INT, MPI_SUM, 0,
               MPI_COMM_WORLD);
    if(rank == 0)
    {
        int n_corr = 0;
        std::vector<int> cmpr = testing.get_labels();
        for(int i = 0; i < res.size(); ++i)
        {
            int pred = (int)round((double)recv_res[i] / (double)all_trees);
            if(cmpr[i] == pred)
                ++n_corr;
        }
        std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";
        delete recv_res;
    }

    MPI_Finalize();
    return 0;
}

