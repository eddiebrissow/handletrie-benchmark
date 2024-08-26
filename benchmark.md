# Benchmark

### Time in seconds

|  Impl |  1000n  | 100000n  | 1000000n  | 10000000n |
|---|---|---|---|---|
| Python's Dict  | 0.0009  | 0.017  | 1.79  |  21.27 | 
|  C++ map std |  0.00145 |  0.338 |  5.352 |  75.307  |  
|  C++ Handle Trie |  0.00114 |  0.154 |  1.906 | 24.600  |  
|  CPython Handle Trie |  0.002 |  0.321 |  3.10 | -  | 
|  PyBind Handle Trie |  0.0134 |  1.55 |  16.22 | -  | 
|  NanoBind Handle Trie |  0.0009 |  0.4637 |  4.69 | -  | 

### Memory in GBs

|  Impl |  1000n  | 100000n  | 1000000n  | 10000000n 
|---|---|---|---|---|
| Python's Dict         | -     | -  | - | - |
|  C++ map std          |  0.0  |  0.0      | 0.474 | - |  
|  C++ Handle Trie      |  0.0  |  0.015    | 1.112 | 4.17 |  
|  CPython Handle Trie  |  -    |  - | - | - | 
|  PyBind Handle Trie   |  -    |  - | - | - | 
|  NanoBind Handle Trie |  -    |  - | - | - | 


## Python Dict 10 times


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

## MAP 10 times

#### Testing 1000 nodes
```
cxx_none_cxx_map = Average: 0.0046075005, STDEV: 0.00026361114443700773, Memory: 0.02
cxx_none_cxx_none = Average: 0.0031516259, STDEV: 0.0001808776614126244, Memory: 0.02
Difference = Average: 0.0014558746000000004, STDEV: 8.273348302438333e-05, Memory: 0.02
```
#### Testing 100000 nodes
```
cxx_none_cxx_map = Average: 0.4851694946, STDEV: 0.0042235798386867736, Memory: 0.028999999999999998
cxx_none_cxx_none = Average: 0.1465934628, STDEV: 0.0004237876365450059, Memory: 0.028
Difference = Average: 0.33857603179999995, STDEV: 0.0037997922021417675, Memory: 0.028499999999999998
```
#### Testing 1000000 nodes
```
cxx_none_cxx_map = Average: 6.8171070933, STDEV: 0.033726245722401016, Memory: 0.0
cxx_none_cxx_none = Average: 1.4649064579, STDEV: 0.03588943975711944, Memory: 0.0
Difference = Average: 5.3522006354, STDEV: 0.002163194034718423, Memory: 0.0
```
#### Testing 10000000 nodes
```
cxx_none_cxx_map = Average: 89.8500795129, STDEV: 0.2925258824114804, Memory: 0.0
cxx_none_cxx_none = Average: 14.5426796628, STDEV: 0.1626955083469463, Memory: 0.0
Difference = Average: 75.3073998501, STDEV: 0.12983037406453413, Memory: 0.0
```

## Handle Trie 10 times

#### Testing 1000 nodes
```
cxx_handletrie_cxx_handletrie = Average: 0.0042881704, STDEV: 0.0002268184533210058, Memory: 0.018000000000000002
cxx_none_cxx_none = Average: 0.003145575, STDEV: 0.0001949221609093116, Memory: 0.018000000000000002
Difference = Average: 0.0011425954000000003, STDEV: 3.1896292411694204e-05, Memory: 0.018000000000000002
```
#### Testing 100000 nodes
```
cxx_handletrie_cxx_handletrie = Average: 0.3014666083, STDEV: 0.009169091816885164, Memory: 0.055
cxx_none_cxx_none = Average: 0.1474459446, STDEV: 0.001127440141728865, Memory: 0.04
Difference = Average: 0.1540206637, STDEV: 0.0080416516751563, Memory: 0.0475
```
#### Testing 1000000 nodes
```
cxx_handletrie_cxx_handletrie = Average: 3.35551038, STDEV: 0.03263835953066646, Memory: 0.734
cxx_none_cxx_none = Average: 1.4487202256, STDEV: 0.008226855609267813, Memory: 0.0
Difference = Average: 1.9067901544, STDEV: 0.024411503921398646, Memory: 0.367
```
#### Testing 10000000 nodes
```
cxx_handletrie_cxx_handletrie = Average: 39.1638301716, STDEV: 0.21439684516778099, Memory: 0.0
cxx_none_cxx_none = Average: 14.5631316817, STDEV: 0.162017505634407, Memory: 0.0
Difference = Average: 24.600698489899997, STDEV: 0.05237933953337398, Memory: 0.0
```