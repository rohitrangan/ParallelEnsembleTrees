#ifndef __PARALLEL_ENSEMBLE_TREES_DATA_H_
#define __PARALLEL_ENSEMBLE_TREES_DATA_H_

#include<vector>

class Data
{
private:
    std::vector< std::vector<int> > features;
    std::vector<int> labels;
    std::vector<double> weights;

public:
    Data();
    Data(std::vector< std::vector<int> > features, std::vector<int> labels);
    Data(std::vector< std::vector<int> > features, std::vector<int> labels,
         std::vector<double> weights);

    std::vector< std::vector<int> > getFeatures();
    std::vector<int> getLabels();
    std::vector<double> getWeights();
};

#endif /* __PARALLEL_ENSEMBLE_TREES_DATA_H_ */

