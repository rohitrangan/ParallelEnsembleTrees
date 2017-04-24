#ifndef __PARALLEL_ENSEMBLE_TREE_NODE_H_
#define __PARALLEL_ENSEMBLE_TREE_NODE_H_

#include <vector>

class Node
{
    int node_depth;
    std::vector<int> node_data_indices;
    std::vector<int> current_feature_indices;
    int best_feature;
    Node *left;
    Node *right;
    int node_label;
};

#endif /* __PARALLEL_ENSEMBLE_TREE_NODE_H_ */
