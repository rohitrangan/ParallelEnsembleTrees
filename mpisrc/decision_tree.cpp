#include "../include/decision_tree.h"

int DecisionTree::get_best_feature(Node *node, Data& training)
{
    std::vector< std::vector<int> > features(training.get_features());
    std::vector<int> labels(training.get_labels());
    std::vector<double> weights(training.get_weights());

    double g_min = DBL_MAX;
    int fmin_id = INT_MIN;

    std::set<int> features_to_use;
    int available_features = node->current_feature_indices.size();
    if(this->subset_features)
    {
        int subset_size = (int)sqrt((double) available_features);
        unsigned seed =
            std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> dist(0, available_features - 1);

        while(subset_size > 0)
        {
            int rand_feature = dist(generator);
            if(node->current_feature_indices.find(rand_feature) !=
               node->current_feature_indices.end())
            {
                features_to_use.insert(rand_feature);
                --subset_size;
            }
        }
    }
    else
    {
        features_to_use = node->current_feature_indices;
    }

    for(int i: features_to_use){
        double c_freq[2][2] = {{0.0}};

        for(int j: node->node_data_indices){
            //c_freq's first/second row means feature absence/presence in the current vector (features[j])
            //c_freq's first/second column means feature belongs to 0th class/1st class
            c_freq[features[j][i]][labels[j]] += weights[j];
        }
        double n_left = c_freq[0][0] + c_freq[0][1];
        double n_right = c_freq[1][0] + c_freq[1][1];

        if(n_left != 0){
            c_freq[0][0] /= n_left;
            c_freq[0][1] /= n_left;
        }

        if(n_right != 0){
            c_freq[1][0] /= n_right;
            c_freq[1][1] /= n_right;
        }
        double g_left = (n_left/(n_left + n_right))*(1.0 - (pow(c_freq[0][0],2) + pow(c_freq[0][1],2)) );
        double g_right = (n_right/(n_left + n_right))*(1.0 - (pow(c_freq[1][0],2) + pow(c_freq[1][1],2)) );

        double g_total = g_left + g_right;
        if (g_total <= g_min){
            g_min = g_total;
            fmin_id = i;
        }
    }

    if(fmin_id == INT_MIN){
        std::cout<<"This should not happen"<<std::endl;
    }

    return fmin_id;
}

int DecisionTree::get_majority_label(Node* node, Data& training)
{
    std::vector<int> labels = training.get_labels();
    int p_count = 0; int n_count = 0;
    for(int i : node->node_data_indices)
    {
        if(labels[i] == 0)
            ++n_count;
        else
            ++p_count;
    }

    if(n_count >= p_count)
        return 0;
    else
        return 1;
}

int DecisionTree::is_pure(Node* node, Data& training)
{
    std::vector<int> labels = training.get_labels();
    int p_count = 0; int n_count = 0;
    for(int i : node->node_data_indices)
    {
        if(labels[i] == 0)
            ++n_count;
        else
            ++p_count;
    }

    if(n_count == 0)
        return 1;
    else if(p_count == 0)
        return 0;
    else
        return -1;
}

Node* DecisionTree::create_tree(Data& training, int depth,
                                std::set<int> data_ind,
                                std::set<int> feature_ind)
{
    if((depth >= this->max_depth) || ((int)data_ind.size() <= this->min_examples))
    {
        Node* n = new Node(depth, data_ind, feature_ind, -1);
        n->node_label = get_majority_label(n, training);
        return n;
    }

    Node* n = new Node(depth, data_ind, feature_ind, -1);
    int pure_val = is_pure(n, training);
    if(pure_val != -1)
    {
        n->node_label = pure_val;
        return n;
    }

    int best_feature = get_best_feature(n, training);
    std::vector< std::vector<int> > features(training.get_features());

    n->best_feature = best_feature;
    std::set<int> left_data;
    std::set<int> right_data;
    for(int i : data_ind)
    {
        if(features[i][best_feature] == 0)
        {
            left_data.insert(i);
        }
        else
        {
            right_data.insert(i);
        }
    }

    feature_ind.erase(best_feature);

    n->left = create_tree(training, depth + 1, left_data, feature_ind);
    n->right = create_tree(training, depth + 1, right_data, feature_ind);

    return n;
}

int DecisionTree::predict_one(Node* node, std::vector<int> feature)
{
    if(node->node_label != -1)
    {
        return node->node_label;
    }
    else
    {
        if(feature[node->best_feature] == 0)
            return predict_one(node->left, feature);
        else
            return predict_one(node->right, feature);
    }
}

DecisionTree::DecisionTree() : max_depth(10), min_examples(10),
                               subset_features(true)
{
    root = NULL;
}

DecisionTree::DecisionTree(int max_depth, int min_examples,
                           bool subset_features)
{
    this->max_depth = max_depth;
    this->min_examples = min_examples;
    this->subset_features = subset_features;
    root = NULL;
}

void DecisionTree::train(Data& training)
{
    std::vector< std::vector<int> > features = training.get_features();
    int n_features = features[0].size();
    int n_data = training.get_dataset_size();

    std::set<int> data_ind;
    std::set<int> feature_ind;
    for(int i = 0; i < n_features; ++i)
    {
        feature_ind.insert(i);
    }
    for(int i = 0; i < n_data; ++i)
    {
        data_ind.insert(i);
    }

    this->root = create_tree(training, 0, data_ind, feature_ind);
}

std::vector<int> DecisionTree::predict(Data& testing)
{
    std::vector< std::vector<int> > features = testing.get_features();
    int n_data = testing.get_dataset_size();

    std::vector<int> result;
    for(int i = 0; i < n_data; ++i)
    {
        result.push_back(predict_one(this->root, features[i]));
    }

    return result;
}

