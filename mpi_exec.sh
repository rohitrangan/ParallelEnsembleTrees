#!/bin/bash

mpirun -n $1 ./ensemble_trees_pl train_test/train_feature.csv train_test/train_labels.csv train_test/test_feature.csv train_test/test_labels.csv $2 $3