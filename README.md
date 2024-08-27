# WIP

## Initial Benchmark Assessment
<!-- |  Impl |  1000n  | 100000n  | 1000000n  | 10000000n | Memory |
|---|---|---|---|---|---|
| Python's Dict  | 0.0009  | 0.017  | 1.79  |  21.27 | 2.4GB |
|  C++ map std |  0.002 |  0.298 |  5.418 |  92.031  | 5.1GB |
|  C++ Handle Trie |  0.002 |  0.127 |  1.729 | 22.765  | 5.1B |
|  CPython Handle Trie |  0.002 |  0.321 |  3.10 | -  | 7.9GB |
|  PyBind Handle Trie |  0.0134 |  1.55 |  16.22 | -  | 2.9GB |
|  NanoBind Handle Trie |  0.0009 |  0.4637 |  4.69 | -  | 2.89GB | -->

### Time average in seconds

|  Impl |  1,000n  | 100,000n  | 1,000,000n  | 10,000,000n |
|---|---|---|---|---|
| Python's Dict         |0.0005|0.08521|1.4708|14.09462| 
|  C++ map std          |0.00145|0.338 |  5.352 |  75.307  |  
|  C++ Handle Trie      |0.00114|0.154 |  1.906 | 24.600  |  
|  CPython Handle Trie  |0.00171|0.496|5.91491|-| 
|  NanoBind Handle Trie |0.00366|0.70037|8.05807|-| 
|  PyBind Handle Trie   |0.01323|1.51958|17.01032|-| 

### Memory average in GBs

|  Impl |  1,000n  | 100,000n  | 1,000,000n  | 10,000,000n 
|---|---|---|---|---|
| Python's Dict         |0.0| 0.002  | 0.446| 5.166 |
|  C++ map std          |0.0|  0.0      | 0.474 | 2.66 |  
|  C++ Handle Trie      |0.0|  0.015    | 1.112 | 4.17 |  
|  CPython Handle Trie  |0.0|   0.809 | 9.211 | - | 
|  NanoBind Handle Trie |0.0|  1.52 | 16.134 | - | 
|  PyBind Handle Trie   |0.0|  1.52 | 1.52 | - | 



### Build C++

```
make build
```

### Build CPython
```
make build-cpython
```


### Build PyBind

```
make build-pybind
```


### Build NanoBind

```
make build-nanobind
```