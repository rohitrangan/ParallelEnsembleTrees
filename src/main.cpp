#include <iostream>

#include "../include/data.h"
#include "../include/node.h"
#include "../include/decision_tree.h"
#include "../include/random_forest.h"

int main(int argc, char* argv[])
{
    Data training(argv[1], argv[2]);
    Data testing(argv[3], argv[4]);

    /*DecisionTree dtree(10, 10, false);
    dtree.train(training);
    std::vector<int> res = dtree.predict(testing);

    int n_corr = 0;
    std::vector<int> cmpr = testing.get_labels();
    for(int i = 0; i < res.size(); ++i)
    {
        if(cmpr[i] == res[i])
            ++n_corr;
    }*/

    //std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";

    RandomForest rf(50, 10, 10, false);
    rf.train(training);
    std::vector<int> res = rf.predict(testing);

    int n_corr = 0;
    std::vector<int> cmpr = testing.get_labels();
    for(int i = 0; i < res.size(); ++i)
    {
        if(cmpr[i] == res[i])
            ++n_corr;
    }

    std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";

    return 0;
}

