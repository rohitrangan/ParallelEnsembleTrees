#include "../include/node.h"

Node::Node(int node_depth, std::set<int> node_data_indices,
           std::set<int> current_feature_indices, int best_feature)
{
    this->node_depth = node_depth;
    this->node_data_indices = node_data_indices;
    this->current_feature_indices = current_feature_indices;
    this->best_feature = best_feature;
    this->left = NULL;
    this->right = NULL;
    this->node_label = -1;
}

