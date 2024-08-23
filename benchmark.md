# Benchmark



|  Impl |  1000n  | 100000n  | 1000000n  | 10000000n | Memory |
|---|---|---|---|---|---|
| Python's Dict  | 0.0009  | 0.017  | 1.79  |  21.27 | 2.4GB |
|  C++ map std |  0.002 |  0.298 |  5.418 |  92.031  | 5.1GB |
|  C++ Handle Trie |  0.002 |  0.127 |  1.729 | 22.765  | 5.1B |
|  CPython Handle Trie |  0.002 |  0.321 |  3.10 | -  | 7.9GB |
|  PyBind Handle Trie |  0.0134 |  1.55 |  16.22 | -  | 2.9GB |
|  NanoBind Handle Trie |  0.0009 |  0.4637 |  4.69 | -  | 2.89GB |


## Python Dict 3 repetitions


#### Testing 1000 nodes

```
benchmark_python_dict_test_dict = Average: 0.04425750366666667, STDEV: 5.0343554943342414e-05
benchmark_python_dict_none = Average: 0.043301026666666666, STDEV: 4.220204289289273e-05
Dict Access/Insert = Average: 0.0009564770000000042, STDEV: 8.141512050449685e-06
```


#### Testing 100000 nodes

```
benchmark_python_dict_test_dict = Average: 4.402010037, STDEV: 0.016687957509430553
benchmark_python_dict_none = Average: 4.259564364333333, STDEV: 0.017346030391306944
Dict Access/Insert = Average: 0.14244567266666674, STDEV: 0.0006580728818763905
```

#### Testing 1000000 nodes
```
benchmark_python_dict_test_dict = Average: 44.82534277366667, STDEV: 0.575082967391361
benchmark_python_dict_none = Average: 43.03066535433334, STDEV: 0.2580378512841851
Dict Access/Insert = Average: 1.7946774193333326, STDEV: 0.3170451161071759
```

#### Testing 10000000 nodes
```
benchmark_python_dict_test_dict = Average: 456.295481414, STDEV: 0.6538481582336224
benchmark_python_dict_none = Average: 435.0223514286667, STDEV: 1.8269940672597447
Dict Access/Insert = Average: 21.2731299853333, STDEV: 1.1731459090261223
```

## MAP

#### Testing 1000 nodes
```
stdlib: 2 millis
stdlib access: 0
```
#### Testing 100000 nodes
```
stdlib: 29 millis
stdlib access: 0
```
#### Testing 1000000 nodes
```
stdlib: 6 secs 42 millis
stdlib access: 5
```
#### Testing 10000000 nodes
```
stdlib: 1 mins 40 secs
stdlib access: 86
```

## Handle Trie

#### Testing 1000 nodes
```
trie: 2 millis
trie access: 0
```
#### Testing 100000 nodes
```
trie: 39 millis
trie access: 0
```
#### Testing 1000000 nodes
```
trie: 3 secs 20 millis
trie access: 1
```
#### Testing 10000000 nodes
```
trie: 37 secs 43 millis
trie access: 21
```