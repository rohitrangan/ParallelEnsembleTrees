#include "../include/data.h"

Data::Data(std::vector< std::vector<int> > features, std::vector<int> labels)
{
    this->features(features);
    this->labels(labels);
    this->dataset_size = labels.size();
    this->weights(this->dataset_size, (1.0 / this->dataset_size));
}

Data::Data(std::vector< std::vector<int> > features, std::vector<int> labels,
           std::vector<double> weights)
{
    this->features(features);
    this->labels(labels);
    this->weights(weights);
    this->dataset_size = labels.size();
}

int Data::get_dataset_size()
{
    return this->dataset_size;
}

std::vector< std::vector<int> > Data::get_features()
{
    return this->features;
}

std::vector<int> Data::get_labels()
{
    return this->labels;
}

std::vector<double> Data::get_weights()
{
    return this->weights;
}

