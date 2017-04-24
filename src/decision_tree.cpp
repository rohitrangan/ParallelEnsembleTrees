#include "../include/decision_tree.h"
#include <vector>
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

int DecisionTree::getBestFeature(Node *node, Data training){
    std::vector< std::vector<int> > features(training.get_features());
    std::vector<int> labels(training.get_labels());

    for(int i: node.current_feature_indices){
        for()
    }


}