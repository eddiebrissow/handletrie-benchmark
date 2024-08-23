# WIP

# Benchmark

|  Impl |  1000n  | 100000n  | 1000000n  | 10000000n | Memory |
|---|---|---|---|---|---|
| Python's Dict  | 0.0009  | 0.017  | 1.79  |  21.27 | 2.49758GB |
|  C++ map std |  0.002 |  0.298 |  5.418 |  92.031  | 5.15558GB |
|  C++ Handle Trie |  0.002 |  0.127 |  1.729 | 22.765  | 5.15558GB |
|  CPython Handle Trie |  0.002 |  0.321 |  3.10 | -  | 7.94629GB |


### Build C++

```
ln -s CMakeListsCXX.txt CMakeLists.txt
cd build
cmake ..
make
```

### Build CPython
```
python setup.py build_ext --inplace

```


### Build PyBind

```
ln -s CMakeListsPyBind.txt CMakeLists.txt
cd build
cmake ..
make
```