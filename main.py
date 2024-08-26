import time
import json
import statistics
from functools import reduce
import handletrie_cpython
import handletrie_nanobind
import handletrie_pybind



import os
import threading
import subprocess
import gc

HANDLE_HASH_SIZE = 33

from ctypes import CDLL

libs = CDLL("libc.so.6")
PID = 0
PIDCXX = None
PIDS = 0
STATS = 1
MEMORY = 0
RUN = "python"
OROUND = 5 # output round


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
        global TIME, RUN
        start_cxx = time.perf_counter_ns()
        start = time.process_time_ns()
        func(*args, **kwargs)
        stop_cxx = time.perf_counter_ns()
        stop = time.process_time_ns()
        if RUN == "c++":
            TIME = ( (stop_cxx -  start_cxx) - (stop - start) ) / 1000000000
        else:
            TIME = (stop - start) / 1000000000
        # print("=======================================================")
        # print(f"{func.__name__}: {TIME}\t Memory: {MEMORY}GB")
        # print("=======================================================")
    return wrapper


def diff(values):
    return round(abs(reduce(lambda a,b: a-b, values)), OROUND)

def mean_round(values):
    return round(statistics.mean(values), OROUND)

def stdev_round(values):
    return round(statistics.stdev(values), OROUND)


def repeat(f, params, n=2):
    check = {}
    for i in range(n):
        for p in params:
            f(**p)
            m = MEMORY
            key = f"{p.get('name')}_{p.get('f').__name__}"
            if key not in check:
                check[key] = {"time": [TIME], "memory": [m]}
            else:
                check[key]["time"].append(TIME)
                check[key]["memory"].append(m)
            gc.collect()

    print("=======================================================")
    diff_m = []
    diff_sdv = []
    memory_list = []
    for k,v in check.items():
        vv = v["time"]
        print(f"{k} = Average: {mean_round(vv)}, STDEV: {stdev_round(vv)}, Memory: {mean_round(v['memory'])} GB")
        diff_m.append(statistics.mean(vv))
        diff_sdv.append(statistics.stdev(vv))
        memory_list.append(statistics.mean(v["memory"]))

    print(f"Difference = Average: {diff(diff_m)}, STDEV: {diff(diff_sdv)}, Memory: {diff(memory_list)} GB")
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
    count = 0
    baseline = {}
    for key_count in {1, 2, 5}:
        str_key = str(key_count)
        key_size: int = (HANDLE_HASH_SIZE - 1) * key_count
        for i in range(n_insertions):
            count += 1
            count += key_size
            s = "".join([R_TLB[libs.rand() % 16] for j in range(key_size)])
            # s = "".join([R_TLB[R[str_key][i][j]] for j in range(key_size)])
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
def benchmark_python_handle_trie(f, n_insertions: int = 1000000, **kwargs):
    count = 0
    for key_count in {1, 2, 5}:
        str_key = str(key_count)
        key_size: int = (HANDLE_HASH_SIZE - 1) * key_count
        if kwargs.get("module"):
            baseline = kwargs["module"].HandleTrie(key_size + 1)
        else:
            baseline = None
        for i in range(n_insertions):
            count += 1
            count += key_size
            s = "".join([R_TLB[libs.rand() % 16] for j in range(key_size)])
            # s = "".join([R_TLB[R[str_key][i][j]] for j in range(key_size)])
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



def cxx_none():
    return "none"

def cxx_handletrie():
    return "handletrie"

def cxx_map():
    return "map"

@measure
def benchmark_cxx(f, n_insertions: int = 1000000, **kwargs):
    subprocess.run(['build/HandleTrie', f(), str(n_insertions)])



def get_pid():
    global PID, PIDCXX, PIDS
    if PIDS == 1:
        ps = subprocess.run('ps -a'.split(' '), stdout=subprocess.PIPE)
        s = str(ps.stdout)
        if 'HandleTrie' in s:
            PIDCXX = next((f for f in s.split('\\n') if 'HandleTrie' in f)).split(' ')[1]
            return PIDCXX
    return PID


def memory_count():
    global STATS, MEMORY
    while True:
        if STATS == 0:
            break
        out = subprocess.run(f"ps -p {get_pid()} -o rss=".split(' '), stdout=subprocess.PIPE)
        try:
            MEMORY = round(int(out.stdout) / 1000000.0, 2)
        except:
            pass
        # print(pid, round(int(out.stdout) / 1000000.0, 2), STATS ) 
        time.sleep(1)





def main():
    global PID, STATS, PIDS, RUN
    PID = os.getpid()
    x = threading.Thread(target=memory_count)
    x.start()

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


    for i in [1000, 100000, 1000000, 10000000]:
        # generate_random(i)
        # print(f"Testing {i} nodes")
        # PIDS = 1
        # RUN = "c++"
        # repeat(benchmark_cxx, [
        #     {'f': cxx_handletrie, 'name': 'cxx_handletrie', 'n_insertions': i},
        #     {'f': cxx_none, 'name': 'cxx_none', 'n_insertions': i},
        # ], 10)

        # print(f"Testing {i} nodes")
        # repeat(benchmark_cxx, [
        #     {'f': cxx_map, 'name': 'cxx_none', 'n_insertions': i},
        #     {'f': cxx_none, 'name': 'cxx_none', 'n_insertions': i},
        # ], 10)

        RUN = "python"
        print(f"Testing {i} nodes")

        repeat(benchmark_python_dict, [
            {'f': test_dict, 'name': 'benchmark_python_dict', 'n_insertions': i}, 
            {'f': none, 'name': 'benchmark_python_dict', 'n_insertions': i}], 
            10)

        print(f"Testing {i} nodes")
        repeat(benchmark_python_handle_trie, [
            {'f': test_cpython_handle_trie, 'name': 'benchmark_handle_trie_cpython', 'n_insertions': i, 'module': handletrie_cpython}, 
            {'f': none, 'name': 'benchmark_handle_trie_cpython', 'n_insertions': i}], 
            10)

        print(f"Testing {i} nodes")
        repeat(benchmark_python_handle_trie, [
            {'f': test_cpython_handle_trie, 'name': 'benchmark_handle_trie_nanobind', 'n_insertions': i, 'module': handletrie_nanobind}, 
            {'f': none, 'name': 'benchmark_handle_trie_nanobind', 'n_insertions': i}], 
            10)
        
        print(f"Testing {i} nodes")
        repeat(benchmark_python_handle_trie, [
            {'f': test_cpython_handle_trie, 'name': 'benchmark_handle_trie_pybind', 'n_insertions': i, 'module': handletrie_pybind}, 
            {'f': none, 'name': 'benchmark_handle_trie_pybind', 'n_insertions': i}], 
            10)

    STATS = 0
    x.join()


main()