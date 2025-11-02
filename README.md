# MultipleChoiceSubsetSumSolver

A solver for the multiple-choice subset sum problem

## Implemented algorithms

* Dynamic programming
  * Bellman
    * Array `--algorithm dynamic-programming-bellman-array`
    * Word RAM (only optimal value) `--algorithm dynamic-programming-bellman-word-ram`
    * Word RAM with recursive scheme `--algorithm dynamic-programming-bellman-word-ram-rec`

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
ctest --parallel --output-on-failure  --test-dir build/test
```
