# MultipleChoiceSubsetSumSolver

A solver for the multiple-choice subset sum problem

## Implemented algorithms

* Dynamic programming
  * Bellman
    * Array `-a dynamic-programming-bellman-array`
    * Word RAM (only optimal value) `-a dynamic-programming-bellman-word-ram`
    * Word RAM with recursive scheme `-a dynamic-programming-bellman-word-ram-rec`

## Usage

### Command line

Compile:
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
cmake --install build --config Release --prefix install
```

Download data:
```shell
python3 scripts/download_data.py
```


Run tests:
```
export MULTIPLE_CHOICE_SUBSET_SUM_DATA=$(pwd)/data
cd build/test
ctest --parallel
```
