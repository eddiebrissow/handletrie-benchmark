import time
import json
import statistics
from functools import reduce
from handletrie import HandleTrie

HANDLE_HASH_SIZE = 33

from ctypes import CDLL

libs = CDLL("libc.so.6")



R = None

TIME = 0


R_TLB = [
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
]


def lcg(modulus: int, a: int, c: int, seed: int):
    """Linear congruential generator."""
    while True:
        seed = (a * seed + c) % modulus
        yield seed

def lcg_to_interval(generator, interval_min: int, interval_max: int):
    range_size = interval_max - interval_min + 1
    
    for value in generator:
        # Map value to the interval [interval_min, interval_max]
        yield interval_min + (value % range_size)

def measure(func):
    def wrapper(*args, **kwargs):
        global TIME
        start = time.process_time_ns()
        func(*args, **kwargs)
        stop = time.process_time_ns()
        TIME = (stop - start) / 1000000000
        print("=======================================================")
        print(f"{func.__name__}: {TIME}")
        print("=======================================================")
    return wrapper


def repeat(f, params, n=2):
    check = {}
    for i in range(n):
        for p in params:
            f(**p)
            key = f"{p.get('name')}_{p.get('f').__name__}"
            if key not in check:
                check[key] = [TIME]
            else:
                check[key].append(TIME)
    print("=======================================================")
    diff_m = []
    diff_sdv = []
    for k,v in check.items():
        print(f"{k} = Average: {statistics.mean(v)}, STDEV: {statistics.stdev(v)}")
        diff_m.append(statistics.mean(v))
        diff_sdv.append(statistics.stdev(v))

    print(f"Difference = Average: {abs(reduce(lambda a,b: a-b, diff_m))}, STDEV: {abs(reduce(lambda a,b: a-b, diff_sdv))}")
    print("=======================================================")


def save_json(data, append=""):
    with open(append + 'data.json', 'w') as f:
        json.dump(data, f)


def load_json(append=""):
    try:
        with open(append + 'data.json', 'r') as f:
            return json.load(f)
    except FileNotFoundError:
        return {}


@measure
def generate_random(interactions=1000000):
    global R
    R = load_json(append=str(interactions))
    if not R:
        for key_count in {1, 2, 5}:
            key_size: int = (HANDLE_HASH_SIZE - 1) * key_count
            key_count = str(key_count)
            R[key_count] = []
            for i in range(interactions):
                R[key_count].append([])
                for j in range(key_size):
                    R[key_count][i].append(libs.rand() % 16)
        save_json(R, append=str(interactions))


def test_dict(baseline, s):
    if not baseline.get(s):
        baseline[s] = 0
    else:
        baseline[s] += 1


def none(baseline, s):
    pass

@measure
def benchmark_python_dict(f, n_insertions: int = 1000000, **kwargs):
    # g = lcg(2**31, 1103515245, 1000, 42)
    count = 0
    baseline = {}
    for key_count in {1, 2, 5}:
        str_key = str(key_count)
        key_size: int = (HANDLE_HASH_SIZE - 1) * key_count
        for i in range(n_insertions):
            count += 1
            count += key_size
            # s = "".join([R_TLB[libs.rand() % 16] for j in range(key_size)])
            s = "".join([R_TLB[R[str_key][i][j]] for j in range(key_size)])
            s = s[:key_size] + '0' + s[key_size + 1:]

            f(baseline, s)



def test_cpython_handle_trie(baseline, s):
    v = baseline.lookup(s)
    if not v:
        baseline.insert(s, 0)
    else:
        v += 1
        baseline.insert(s, v)



@measure
def benchmark_cpython_handle_trie(f, n_insertions: int = 1000000, **kwargs):
    # g = lcg(2**31, 1103515245, 1000, 42)
    count = 0
    # baseline = {}
    for key_count in {1, 2, 5}:
        str_key = str(key_count)
        key_size: int = (HANDLE_HASH_SIZE - 1) * key_count
        baseline = HandleTrie(key_size + 1)
        for i in range(n_insertions):
            count += 1
            count += key_size
            # s = "".join([R_TLB[libs.rand() % 16] for j in range(key_size)])
            s = "".join([R_TLB[R[str_key][i][j]] for j in range(key_size)])
            s = s[:key_size] + '0' + s[key_size + 1:]

            f(baseline, s)


# estrutura com mais dados e teste mais longo, --gil
# rodar varios nodos

def list_test(a=[]):
    b = [i for i in a]


def no_list(a=None):
    # a = a or []
    b = [] if a is None else [i for i in a]
    # if a is None:
    #     b = []
    # else:
    #     b = [i for i in a]


@measure
def aaa(f):
    for i in range(1000000):
        f()


@measure
def bbb(f):
    for i in range(1000000):
        f([1, 2])


def main():
    # global TIME

    # a = HandleTrie(5)
    # a.insert("aaaaa", "1")
    # a.insert("aaaab", "2")
    # a.insert("aaaac", "3")

    # print('lookip')
    # print(a.lookup("aaazc"))

    # print(a.lookup("aaaaa"))
    # print(a.lookup("aaaab"))
    # print(a.lookup("aaazc"))

    # [1000, 100000, 1000000, 10000000, 1000000000]

    # for i in [1000, 100000, 1000000]:
    #     generate_random(i)
    #     print(f"Testing {i} nodes")
    #     repeat(benchmark_python_dict, [
    #         {'f': test_dict, 'name': 'benchmark_python_dict', 'n_insertions': i}, 
    #         {'f': none, 'name': 'benchmark_python_dict', 'n_insertions': i}], 
    #         3)


    # for i in [1000, 100000, 1000000]:
    #     generate_random(i)
    #     print(f"Testing {i} nodes")
    #     repeat(benchmark_cpython_handle_trie, [
    #         {'f': test_cpython_handle_trie, 'name': 'benchmark_cpython_handle_trie', 'n_insertions': i}, 
    #         {'f': none, 'name': 'benchmark_cpython_handle_trie', 'n_insertions': i}], 
    #         3)

    

    # benchmark_python_dict(test_dict)
    # t_dict = TIME
    # benchmark_python_dict(none)
    # t_none = TIME
    # print(t_dict - t_none)


main()