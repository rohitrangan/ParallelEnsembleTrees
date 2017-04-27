#include <string>
#include <iostream>

#include "../include/data.h"
#include "../include/node.h"
#include "../include/adaboost.h"
#include "../include/decision_tree.h"
#include "../include/random_forest.h"

int main(int argc, char* argv[])
{
    if(argc != 7)
    {
        std::cout << "Need 6 arguments.\n";
        return 1;
    }

    Data training(argv[1], argv[2]);
    Data testing(argv[3], argv[4]);
    int alg = std::stoi(argv[5]);
    int num_trees = std::stoi(argv[6]);

    if(alg == 1)
    {
        //Start clock here
        DecisionTree dtree(10, 10, false);
        dtree.train(training);
        std::vector<int> res = dtree.predict(testing);

        int n_corr = 0;
        std::vector<int> cmpr = testing.get_labels();
        for(int i = 0; i < (int)res.size(); ++i)
        {
            if(cmpr[i] == res[i])
                ++n_corr;
        }

        //std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";
        //End clock here
    }
    else if(alg == 2)
    {
        RandomForest rf(num_trees, 10, 10, false);
        rf.train(training);
        std::vector<int> res = rf.predict(testing);

        int n_corr = 0;
        std::vector<int> cmpr = testing.get_labels();
        for(int i = 0; i < (int)res.size(); ++i)
        {
            if(cmpr[i] == res[i])
                ++n_corr;
        }

        // std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";
    }
    else if(alg == 3)
    {
        AdaBoost ab(num_trees, 10, 10);
        ab.train(training);
        std::vector<int> res = ab.predict(testing);

        int n_corr = 0;
        std::vector<int> cmpr = testing.get_labels();
        for(int i = 0; i < (int)res.size(); ++i)
        {
            if(cmpr[i] == res[i])
                ++n_corr;
        }
        //Output number of trees and time taken
        // std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";
    }

    return 0;
}

