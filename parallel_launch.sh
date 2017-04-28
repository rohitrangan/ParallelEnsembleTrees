#!/bin/bash

tree_type=2
max_tree_type=3
num_trees=2
max_trees=256
nproc=2
maxnproc=4

while [ $tree_type -le $max_tree_type ]; do
    echo "Hello"
    while [ $num_trees -le $max_trees ]; do
        ((num_trees = 2*num_trees))
        while [ $nproc -le $num_trees ]; do            
            echo "$tree_type $nproc $num_trees"
            sc="mpirun -n $nproc ./ensemble_trees_pl train_test/train_feature.csv train_test/train_labels.csv train_test/test_feature.csv train_test/test_labels.csv $tree_type $num_trees >>parallel2.paradise"
            eval $sc
            ((nproc = 2*nproc))
        done
        ((nproc = 2))    
    done
    ((num_trees = 2))
    ((tree_type = tree_type + 1))
done