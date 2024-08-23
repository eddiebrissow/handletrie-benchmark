# WIP

# Benchmark
|  Impl |  1000n  | 100000n  | 1000000n  | 10000000n | Memory |
|---|---|---|---|---|---|
| Python's Dict  | 0.0009  | 0.017  | 1.79  |  21.27 | 2.4GB |
|  C++ map std |  0.002 |  0.298 |  5.418 |  92.031  | 5.1GB |
|  C++ Handle Trie |  0.002 |  0.127 |  1.729 | 22.765  | 5.1B |
|  CPython Handle Trie |  0.002 |  0.321 |  3.10 | -  | 7.9GB |
|  PyBind Handle Trie |  0.0134 |  1.55 |  16.22 | -  | 2.9GB |
|  NanoBind Handle Trie |  0.0009 |  0.4637 |  4.69 | -  | 2.89GB |



### Build C++

```
make compile
```

### Build CPython
```
make compile-cpython

```


### Build PyBind

```
make compile-pybind
```


### Build NanoBind

```
make compile-nanobind
```