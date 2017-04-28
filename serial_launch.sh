#!/bin/bash

tree_type=2
max_tree_type=3
num_trees=2
max_trees=256

while [ $tree_type -le $max_tree_type ]; do
    echo "Hello"
    while [ $num_trees -le $max_trees ]; do

        ((num_trees = 2*num_trees))
        echo "$tree_type $num_trees"
        sc="./ensemble_trees train_test/train_feature.csv train_test/train_labels.csv train_test/test_feature.csv train_test/test_labels.csv $tree_type $num_trees >>serial.dat"
        eval $sc
    done
    ((num_trees = 2))
    ((tree_type = tree_type + 1))
done