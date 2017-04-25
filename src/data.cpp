#include "../include/data.h"

Data::Data(char* feature_file, char* label_file)
{
    std::string line;
    char delim = ',';
    std::ifstream feature_stream;
    feature_stream.open(feature_file, std::ios::in);
    while(getline(feature_stream, line))
    {
        std::vector<int> val;
        std::string::size_type i = 0;
        std::string::size_type j = line.find(delim);

        while(j != std::string::npos) {
            val.push_back(std::stoi(line.substr(i, j-i)));
            i = ++j;
            j = line.find(delim, j);

            if(j == std::string::npos)
                val.push_back(std::stoi(line.substr(i, line.length())));
        }

        this->features.push_back(val);
    }
    feature_stream.close();


    std::ifstream label_stream;
    label_stream.open(label_file, std::ios::in);
    while(getline(label_stream, line))
    {
        std::string::size_type i = 0;
        std::string::size_type j = line.find(delim);

        while(j != std::string::npos) {
            this->labels.push_back(std::stoi(line.substr(i, j-i)));
            i = ++j;
            j = line.find(delim, j);

            if(j == std::string::npos)
                this->labels.push_back(std::stoi(line.substr(i, line.length())));
        }
    }
    label_stream.close();

    this->dataset_size = labels.size();
    this->weights =
        std::vector<double>(this->dataset_size, (1.0 / this->dataset_size));
}

Data::Data(std::vector< std::vector<int> > features, std::vector<int> labels)
{
    this->features = features;
    this->labels = labels;
    this->dataset_size = labels.size();
    this->weights =
        std::vector<double>(this->dataset_size, (1.0 / this->dataset_size));
}

Data::Data(std::vector< std::vector<int> > features, std::vector<int> labels,
           std::vector<double> weights)
{
    this->features = features;
    this->labels = labels;
    this->weights = weights;
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

