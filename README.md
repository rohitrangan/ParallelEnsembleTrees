# ParallelEnsembleTrees

Parallelized versions of AdaBoost and Random Forests.

## Requirements

1. C++ compiler supporting C++11.
2. GNU Make.
3. Any implementation of MPI.

## Compiling

1. Run 'make'.
2. The serial executable file is named 'ensemble\_trees' and the parallel
   executable file is named 'ensemble\_trees\_pl'.

## Execution

Both the serial and the parallel versions of the program require 6 command-line
arguments. They are, in order -

1. Training dataset features file path (CSV, each line is a feature vector).
2. Training dataset labels file path (CSV, one line with the labels).
3. Testing dataset features file path (CSV, each line is a feature vector).
4. Testing dataset labels file path (CSV, one line with the labels).
5. An integer specifying which algorithm to run (2 - Random Forest, 3 - AdaBoost)
6. An integer specifying the number of trees to learn.

