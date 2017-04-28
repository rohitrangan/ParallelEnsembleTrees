#include <string>
#include <iostream>
#include <sys/time.h>

#include "../include/data.h"
#include "../include/node.h"
#include "../include/adaboost.h"
#include "../include/decision_tree.h"
#include "../include/random_forest.h"

typedef unsigned long long timestamp_t;
static timestamp_t get_timestamp ();
static timestamp_t
    get_timestamp ()
    {
      struct timeval now;
      gettimeofday (&now, NULL);
      return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }

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
        timestamp_t t0 = get_timestamp();
        
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
        timestamp_t t1 = get_timestamp();

        std::cout<<(t1 - t0)<<std::endl;
        //End clock here
    }
    else if(alg == 2)
    {
        timestamp_t t0 = get_timestamp();
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
        timestamp_t t1 = get_timestamp();

        std::cout<<alg<<","<<num_trees<<","<<(t1 - t0)<<std::endl;
        // std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";
    }
    else if(alg == 3)
    {
        timestamp_t t0 = get_timestamp();
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
        timestamp_t t1 = get_timestamp();

        std::cout<<alg<<","<<num_trees<<","<<(t1 - t0)<<std::endl;
        //Output number of trees and time taken
        // std::cout << "Result = " << (double)n_corr / (double)res.size() << "\n";
    }

    return 0;
}

