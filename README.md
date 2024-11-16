# TSP (Travelling salesman problem)

### https://en.wikipedia.org/wiki/Travelling_salesman_problem



### 1.Config:

* Place in programe arguments an arg with path to config file or left empty and use default config if exists,

### 1.1 Config file:

> Config file should contain info about input file or dir path ( if need test more than one instance ), output file path and valid JSON line. JSON line can contains elements like:
> {"checkAllNodes": `bolean`, nodesList: `int[]` "permutations": `int`, "maxDuration": `int`, "repeatNumber": `int`, "print": `bool`}
 

##### Example config file.
```
./data/data.txt
./output/output.txt
{"checkAllNodes": true, "permutations": 5000, "maxDuration": 600000, "repeatNumber": 10, "print": false}
```

