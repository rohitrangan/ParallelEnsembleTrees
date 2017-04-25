#ifndef __PARALLEL_ENSEMBLE_TREES_DATA_H_
#define __PARALLEL_ENSEMBLE_TREES_DATA_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Data
{
private:
    std::vector< std::vector<int> > features;
    std::vector<int> labels;
    std::vector<double> weights;
    int dataset_size;

public:
    Data();
    Data(char* feature_file, char* label_file);
    Data(std::vector< std::vector<int> > features, std::vector<int> labels);
    Data(std::vector< std::vector<int> > features, std::vector<int> labels,
         std::vector<double> weights);

    int get_dataset_size();
    std::vector< std::vector<int> > get_features();
    std::vector<int> get_labels();
    std::vector<double> get_weights();
};

#endif /* __PARALLEL_ENSEMBLE_TREES_DATA_H_ */

