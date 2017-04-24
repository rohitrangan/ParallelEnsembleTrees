#include "../include/data.h"

Data::Data(std::vector< std::vector<int> > features, std::vector<int> labels)
{
    this->features(features);
    this->labels(labels);
    this->weights(labels.size(), (1.0 / labels.size()));
}

Data::Data(std::vector< std::vector<int> > features, std::vector<int> labels,
           std::vector<double> weights)
{
    this->features(features);
    this->labels(labels);
    this->weights(weights);
}

std::vector< std::vector<int> > Data::getFeatures()
{
    return this->features;
}

std::vector<int> Data::getLabels()
{
    return this->labels;
}

std::vector<double> Data::getWeights()
{
    return this->weights;
}

