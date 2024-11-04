# TSP (Travelling salesman problem)

### https://en.wikipedia.org/wiki/Travelling_salesman_problem



### 1.Config:

* Place in programe arguments an arg with path to config file or left empty and use default config if exists,

### 1.1 Config file:

> Config file should contain info about input file or dir path ( if need test more than one instance ), output file path, starting node (may be an array of starting nodes) `0 1 2 3 4 5` or `0` or `all` to indicate to check all nodes as starting nodes, number of repeats on one instance, flag `[ 0, 1 ]` - for `[ disable prints, enable print ]`. Each element in new line ( separate by `[ ENTER ]` ).

##### Example config file.
```
./data/data.txt
./output/output.txt
1 2 3 4 5
10
1
```

