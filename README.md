# WIP

## Benchmark 
<!-- |  Impl |  1000n  | 100000n  | 1000000n  | 10000000n | Memory |
|---|---|---|---|---|---|
| Python's Dict  | 0.0009  | 0.017  | 1.79  |  21.27 | 2.4GB |
|  C++ map std |  0.002 |  0.298 |  5.418 |  92.031  | 5.1GB |
|  C++ Handle Trie |  0.002 |  0.127 |  1.729 | 22.765  | 5.1B |
|  CPython Handle Trie |  0.002 |  0.321 |  3.10 | -  | 7.9GB |
|  PyBind Handle Trie |  0.0134 |  1.55 |  16.22 | -  | 2.9GB |
|  NanoBind Handle Trie |  0.0009 |  0.4637 |  4.69 | -  | 2.89GB | -->

### Time average in seconds

|  Impl |  1,000n  | 100,000n  | 1,000,000n  | 10,000,000n |60,000,000 |
|---|---|---|---|---|---|
| Python's Dict         |0.0005|0.08521|1.4708|19.10467| 79.20437|
|  C++ map std          |0.00145|0.338 |5.352|75.307| 561.42147 |
|  C++ Handle Trie      |0.00114|0.154 |1.906|24.600| 149.15487 |
|  CPython Handle Trie  |0.00171|0.496|5.91491|69.91068|510.58422| 
|  NanoBind Handle Trie |0.00366|0.70037|8.05807|81.08409|-| 
|  PyBind Handle Trie   |0.01323|1.51958|20.47771|170.97056|-| 

### Memory average in GBs

|  Impl |  1,000n  | 100,000n  | 1,000,000n  | 10,000,000n |60,000,000n |
|---|---|---|---|---|---|
| Python's Dict         |0.0|0.002|0.267|3.108| 20.59636|
|  C++ map std          |0.0|0.0|0.474|2.66| - |
|  C++ Handle Trie      |0.0|0.015|1.112|4.17| - | 
|  CPython Handle Trie  |0.0|0.809|0.552|5.675| 33.04674|
|  NanoBind Handle Trie |0.0|1.52|0.604|5.927| - |
|  PyBind Handle Trie   |0.0|1.52|0.62|5.772| - |



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