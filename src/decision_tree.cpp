#include "../include/decision_tree.h"
#include <vector>
#include <limits.h>
#include <math.h>
// class DecisionTree
// {
// private:
//     int max_depth;
//     int min_examples;
//     bool subset_features;
//     Node *root;

//     int getBestFeature(Node *node);

// public:
//     DecisionTree();
//     DecisionTree(int max_depth, int min_examples, bool subset_features);

//     void train(Data training);
//     std::vector<int> predict(Data testing);
// };

// class Node
// {
//     int node_depth;
//     std::vector<int> node_data_indices;
//     std::vector<int> current_feature_indices;
//     int best_feature;
//     Node *left;
//     Node *right;
//     int node_label;
// };

int DecisionTree::get_best_feature(Node *node, Data training){
    std::vector< std::vector<int> > features(training.get_features());
    std::vector<int> labels(training.get_labels());

    
    float g_min = FLT_MAX;
    int fmin_id = INT_MIN;

    for(int i: node.current_feature_indices){
        int c_freq[2][2] = {{0}};
        
        for(int j: node.node_data_indices){
            //c_freq's first/second row means feature absence/presence in the current vector (features[j])
            //c_freq's first/second column means feature belongs to 0th class/1st class
            c_freq[features[j][i]][labels[j]] += 1;
        }
        float n_left = c_freq[0][0] + c_freq[0][1];
        float n_right = c_freq[1][0] + c_freq[1][1];

        if(n_left != 0){
            c_freq[0][0] /= n_left; 
            c_freq[0][1] /= n_left;
        }

        if(n_right != 0){
            c_freq[1][0] /= n_right; 
            c_freq[1][1] /= n_right;
        }
        float g_left = (n_left/(n_left + n_right))*(1.0 - (pow(c_freq[0][0],2) + pow(c_freq[0][1],2)) );
        float g_right = (n_right/(n_left + n_right))*(1.0 - (pow(c_freq[1][0],2) + pow(c_freq[1][1],2)) );

        float g_total = g_left + g_right
        if (g_total <= g_min){
            g_min = g_total
            fmin_id = f_id
        }
    }

    if(fmin_id == INT_MIN){
        cout<<"This should not happen"<<endl;
    }
    
    return fmin_id;
}