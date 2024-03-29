#ifndef __PARALLEL_ENSEMBLE_TREE_NODE_H_
#define __PARALLEL_ENSEMBLE_TREE_NODE_H_

#include <set>
#include <vector>

class Node
{
public:
    int node_depth;
    std::set<int> node_data_indices;
    std::set<int> current_feature_indices;
    int best_feature;
    Node *left;
    Node *right;
    int node_label;

    Node(int node_depth, std::set<int> node_data_indices,
         std::set<int> current_feature_indices, int best_feature);
};

#endif /* __PARALLEL_ENSEMBLE_TREE_NODE_H_ */

