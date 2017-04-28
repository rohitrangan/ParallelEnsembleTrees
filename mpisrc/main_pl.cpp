#include <mpi.h>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "../include/data.h"
#include "../include/node.h"
#include "../include/decision_tree.h"

#define map_pred(x) (((x) == 0) ? -1.0 : 1.0)
#define map_pred_int(x) (((x) == 0) ? -1 : 1)

typedef unsigned long long timestamp_t;
static timestamp_t get_timestamp ();
static timestamp_t
    get_timestamp ()
    {
      struct timeval now;
      gettimeofday (&now, NULL);
      return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }

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

    for(int i = 0; i < (int)training.get_dataset_size(); ++i)
    {
        int data_select = dist(generator);
        new_features.push_back(features[data_select]);
        new_labels.push_back(labels[data_select]);
        new_weights.push_back(weights[data_select]);
    }

    return Data(new_features, new_labels, new_weights);
}

std::vector<DecisionTree> train_rf(Data training, int num_trees, int max_depth,
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

std::vector<int> predict_rf(Data testing, std::vector<DecisionTree> trees)
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

    return results;
}

struct AB_tree
{
    DecisionTree tree;
    double alpha;

    AB_tree(DecisionTree t, double a) : tree(t), alpha(a) {}

    friend bool operator<(const AB_tree& l, const AB_tree& r)
    {
        return (l.alpha < r.alpha);
    }
};

std::vector<AB_tree> train_ab(Data training, int num_trees, int max_depth,
                              int min_examples)
{
    int dataset_size = training.get_dataset_size();
    std::vector<double> data_weights(dataset_size, (1.0 / dataset_size));
    std::vector<int> data_labels(training.get_labels());
    std::vector<AB_tree> trees;

    for(int i = 0; i < num_trees; ++i)
    {
        double eps_err = 0.0, alpha = 0.0, wt_sum = 0.0;
        DecisionTree dtree(max_depth, min_examples, false);
        dtree.train(training, data_weights);
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
        AB_tree ab_tree = {dtree, alpha};
        trees.push_back(ab_tree);
    }
    std::sort(trees.begin(), trees.end());

    return trees;
}

std::vector<int> predict_ab(std::vector<int> f, std::vector<AB_tree> trees)
{
    std::vector<int> preds(trees.size(), 0);
    for(int i = 0; i < (int)trees.size(); ++i)
    {
        preds[i] = map_pred_int(trees[i].tree.predict_one(f));
    }
    return preds;
}

int main(int argc, char* argv[])
{
    if(argc != 7)
    {
        std::cout << "Need 6 arguments.\n";
        return 1;
    }

    int rank, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    int alg = std::stoi(argv[5]);
    int all_trees = std::stoi(argv[6]);

    if(alg == 2)
    {
        int num_trees_proc = all_trees / nproc;
        int max_depth = 10;
        int min_examples = 10;
        bool subset_features = true;

        Data training(argv[1], argv[2]);
        Data testing(argv[3], argv[4]);
        //Start clock here
        timestamp_t t0;
        if(rank == 0){
            t0 = get_timestamp();
        }

        std::vector<DecisionTree> trees =
            train_rf(training, num_trees_proc, max_depth, min_examples,
                     subset_features);
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
            for(int i = 0; i < (int)res.size(); ++i)
            {
                int pred = (int)round((double)recv_res[i] / (double)all_trees);
                if(cmpr[i] == pred)
                    ++n_corr;
            }
            //End time here
            timestamp_t t1 = get_timestamp();

            cout<< "Number of trees "<< all_trees << "time " << (t1 - t0)<<endl;
            //std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";
        }
    }
    else if(alg == 3)
    {
        int num_trees = all_trees;
        int max_depth = 10;
        int min_examples = 10;
        int min_dset_size = 200;
        Data all_training(argv[1], argv[2]);
        Data testing(argv[3], argv[4]);
        int all_dataset_size = all_training.get_dataset_size();
        std::vector< std::vector<int> > all_features =
            all_training.get_features();
        std::vector<int> all_labels = all_training.get_labels();
        std::vector< std::vector<int> > features;
        std::vector<int> labels;
        //Start clock here
        timestamp_t t0;
        if(rank == 0){ 
            t0 = get_timestamp();
        }

        if((all_dataset_size / nproc) < min_dset_size)
        {
            // Get min_dset_size random examples.
            unsigned seed =
                std::chrono::system_clock::now().time_since_epoch().count();
            std::default_random_engine generator(seed);
            std::uniform_int_distribution<int> dist(0, all_dataset_size - 1);

            for(int i = 0; i < min_dset_size; ++i)
            {
                int data_select = dist(generator);
                features.push_back(all_features[data_select]);
                labels.push_back(all_labels[data_select]);
            }
        }
        else
        {
            // Get all_dataset_size / nproc examples.
            int offset = rank * (all_dataset_size / nproc);
            for(int i = offset; i < offset + (all_dataset_size / nproc); ++i)
            {
                features.push_back(all_features[i]);
                labels.push_back(all_labels[i]);
            }
        }

        Data training(features, labels);
        std::vector<AB_tree> trees = train_ab(training, num_trees, max_depth,
                                              min_examples);
        std::vector<double> alphas(num_trees, 0.0);
        for(int i = 0; i < num_trees; ++i)
        {
            alphas[i] = trees[i].alpha;
        }
        double* recv_alphas = nullptr;
        if(rank == 0)
        {
            recv_alphas = new double(num_trees);
        }
        MPI_Reduce(&alphas[0], recv_alphas, num_trees, MPI_DOUBLE, MPI_SUM, 0,
                   MPI_COMM_WORLD);
        if(rank == 0)
        {
            for(int i = 0; i < num_trees; ++i)
            {
                recv_alphas[i] = recv_alphas[i] / (double)nproc;
            }
        }

        int n_corr = 0;
        int testing_size = testing.get_dataset_size();
        std::vector< std::vector<int> > test_features = testing.get_features();
        std::vector<int> test_labels;
        int* reduce_pred = nullptr;
        if(rank == 0)
        {
            reduce_pred = new int(num_trees);
            test_labels = testing.get_labels();
        }
        for(int i = 0; i < testing_size; ++i)
        {
            std::vector<int> res_one = predict_ab(test_features[i], trees);
            MPI_Reduce(&res_one[0], reduce_pred, num_trees, MPI_INT, MPI_SUM, 0,
                       MPI_COMM_WORLD);
            if(rank == 0)
            {
                double final_pred = 0.0;
                for(int j = 0; j < num_trees; ++j)
                {
                    if(reduce_pred[j] != 0)
                    {
                        final_pred += (recv_alphas[j] * reduce_pred[j]);
                    }
                }
                int fin_pred = (final_pred >= 0.0) ? 1 : 0;
                if(fin_pred == test_labels[i])
                {
                    ++n_corr;
                }
            }
        }
        if(rank == 0)
        {
            //End time here
            timestamp_t t1 = get_timestamp();
            cout<< "Number of trees "<< all_trees << "time " << (t1 - t0)<<endl;
            //std::cout << "Result = " << (double)n_corr / (double)testing_size << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}

