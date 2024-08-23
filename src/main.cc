#include <cstdlib>
#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <map>
#include <algorithm>
#include <numeric>

#include "Utils.h"
#include "expression_hasher.h"
#include "HandleTrie.h"

using namespace attention_broker_server;
using namespace std;

#define HANDLE_SPACE_SIZE ((unsigned int) 100)

using namespace attention_broker_server;
using namespace std;


template<typename T>
T variance(const std::vector<T> &vec) {
    const size_t sz = vec.size();
    if (sz <= 1) {
        return 0.0;
    }

    // Calculate the mean
    const T mean = std::accumulate(vec.begin(), vec.end(), 0.0) / sz;

    // Now calculate the variance
    auto variance_func = [&mean, &sz](T accumulator, const T& val) {
        return accumulator + ((val - mean)*(val - mean) / (sz - 1));
    };

    return std::accumulate(vec.begin(), vec.end(), 0.0, variance_func);
}

class TestValue: public HandleTrie::TrieValue {
    public:
        unsigned int count;
        TestValue(int count = 1) {
            this->count = count;
        }
        void merge(TrieValue *other) {

        }

        std::string to_string(){
            return std::to_string(this->count);
        }
};

class AccumulatorValue: public HandleTrie::TrieValue {
    public:
        unsigned int count;
        AccumulatorValue() {
            this->count = 1;
        }
        void merge(TrieValue *other) {
            count += ((AccumulatorValue *) other)->count;
        }
};

char R_TLB[16] = {
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
};

bool visit1(HandleTrie::TrieNode *node, void *data) {
    TestValue *value = (TestValue *) node->value;
    value->count += *((unsigned int *) data);
    return false;
}

bool visit2(HandleTrie::TrieNode *node, void *data) {
    TestValue *value = (TestValue *) node->value;
    value->count += 1;
    return false;
}

void visitor3(HandleTrie *trie, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        trie->traverse(Utils::flip_coin(), &visit2, NULL);
    }
}

void EXPECT_TRUE(bool value){
    if(!value){
        throw std::runtime_error("Invalid role");
    }
}


void EXPECT_EQ(auto value1, auto value2){
    const char* v1 = (const char*) value1;
    const char* v2 = (const char*) value2;

    if (value1 != value2){
        for(int i=0; i< strlen(v1); i++){
            if (v1[i] == v2[i]){
                cout << "Equal: " << v1[i] << " " << v2[i] << endl;
            }else{
                cout << "Not Equal: " << v1[i] << " " << v2[i] << endl;
            }
        }
        throw std::runtime_error("Not equal");
    }
}


void test1(){
    HandleTrie trie(4);
    TestValue *value;

    trie.insert("ABCD", new TestValue(3));
    value = (TestValue *) trie.lookup("ABCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 3);
    value = (TestValue *) trie.lookup("ABCF");
    EXPECT_TRUE(value == NULL);

    trie.insert("ABCF", new TestValue(4));
    value = (TestValue *) trie.lookup("ABCF");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 4);
    value = (TestValue *) trie.lookup("ABCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 3);

    trie.insert("ABFD", new TestValue(5));
    value = (TestValue *) trie.lookup("ABFD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 5);

    trie.insert("FBCD", new TestValue(6));
    value = (TestValue *) trie.lookup("FBCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 6);

    trie.insert("AFCD", new TestValue(7));
    value = (TestValue *) trie.lookup("AFCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 7);

    value = (TestValue *) trie.lookup("ABCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 3);
    value = (TestValue *) trie.lookup("ABCF");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 4);
    value = (TestValue *) trie.lookup("ABFD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 5);
    value = (TestValue *) trie.lookup("FBCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 6);
    value = (TestValue *) trie.lookup("AFCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 7);

    value = (TestValue *) trie.lookup("ABFF");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("AFCF");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("AFFD");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("FBCF");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("FBFD");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("FFCD");
    EXPECT_TRUE(value == NULL);

    unsigned int delta = 7;
    trie.traverse(false, &visit1, &delta);
    value = (TestValue *) trie.lookup("ABCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 3 + delta);
    value = (TestValue *) trie.lookup("ABCF");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 4 + delta);
    value = (TestValue *) trie.lookup("ABFD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 5 + delta);
    value = (TestValue *) trie.lookup("FBCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 6 + delta);
    value = (TestValue *) trie.lookup("AFCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 7 + delta);
}


void test2(){
     HandleTrie trie(4);
    TestValue *value;

    trie.insert("ABCD", new TestValue(3));
    value = (TestValue *) trie.lookup("ABCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 3);
    value = (TestValue *) trie.lookup("ABCF");
    EXPECT_TRUE(value == NULL);

    trie.insert("ABCF", new TestValue(4));
    value = (TestValue *) trie.lookup("ABCF");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 4);
    value = (TestValue *) trie.lookup("ABCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 3);

    trie.insert("ABFD", new TestValue(5));
    value = (TestValue *) trie.lookup("ABFD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 5);

    trie.insert("FBCD", new TestValue(6));
    value = (TestValue *) trie.lookup("FBCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 6);

    trie.insert("AFCD", new TestValue(7));
    value = (TestValue *) trie.lookup("AFCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 7);

    value = (TestValue *) trie.lookup("ABCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 3);
    value = (TestValue *) trie.lookup("ABCF");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 4);
    value = (TestValue *) trie.lookup("ABFD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 5);
    value = (TestValue *) trie.lookup("FBCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 6);
    value = (TestValue *) trie.lookup("AFCD");
    EXPECT_TRUE(value != NULL);
    EXPECT_TRUE(value->count == 7);

    value = (TestValue *) trie.lookup("ABFF");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("AFCF");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("AFFD");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("FBCF");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("FBFD");
    EXPECT_TRUE(value == NULL);
    value = (TestValue *) trie.lookup("FFCD");
    EXPECT_TRUE(value == NULL);

    vector<thread *> visitors;
    unsigned int n_visits = 100000;
    unsigned int n_threads = 32;
    for (unsigned int i = 0; i < n_threads; i++) {
        visitors.push_back(new thread(&visitor3, &trie, n_visits));
    }
    for (thread *t: visitors) {
        t->join();
    }

    value = (TestValue *) trie.lookup("ABCD");
    EXPECT_TRUE(value->count == 3 + n_visits * n_threads);
    value = (TestValue *) trie.lookup("ABCF");
    EXPECT_TRUE(value->count == 4 + n_visits * n_threads);
    value = (TestValue *) trie.lookup("ABFD");
    EXPECT_TRUE(value->count == 5 + n_visits * n_threads);
    value = (TestValue *) trie.lookup("FBCD");
    EXPECT_TRUE(value->count == 6 + n_visits * n_threads);
    value = (TestValue *) trie.lookup("AFCD");
    EXPECT_TRUE(value->count == 7 + n_visits * n_threads);
}


void test3(){
  HandleTrie trie(4);

    trie.insert("ABCD", new AccumulatorValue());
    trie.insert("ABCF", new AccumulatorValue());
    trie.insert("ABFD", new AccumulatorValue());
    trie.insert("ABFF", new AccumulatorValue());
    trie.insert("AFCD", new AccumulatorValue());
    trie.insert("AFCF", new AccumulatorValue());
    trie.insert("AFFD", new AccumulatorValue());
    trie.insert("AFFF", new AccumulatorValue());
    trie.insert("FBCD", new AccumulatorValue());
    trie.insert("FBCF", new AccumulatorValue());
    trie.insert("FBFD", new AccumulatorValue());
    trie.insert("FBFF", new AccumulatorValue());
    trie.insert("FFCD", new AccumulatorValue());
    trie.insert("FFCF", new AccumulatorValue());
    trie.insert("FFFD", new AccumulatorValue());
    trie.insert("FFFF", new AccumulatorValue());

    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABCD"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABCF"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABFD"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABFF"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("AFCD"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("AFCF"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("AFFD"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("AFFF"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FBCD"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FBCF"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FBFD"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FBFF"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFCD"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFCF"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFFD"))->count == 1);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFFF"))->count == 1);

    trie.insert("ABFF", new AccumulatorValue());
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABFF"))->count == 2);
    trie.insert("FFCD", new AccumulatorValue());
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFCD"))->count == 2);

    trie.insert("ABCD", new AccumulatorValue());
    trie.insert("ABCF", new AccumulatorValue());
    trie.insert("ABFD", new AccumulatorValue());
    trie.insert("ABFF", new AccumulatorValue());
    trie.insert("AFCD", new AccumulatorValue());
    trie.insert("AFCF", new AccumulatorValue());
    trie.insert("AFFD", new AccumulatorValue());
    trie.insert("AFFF", new AccumulatorValue());
    trie.insert("FBCD", new AccumulatorValue());
    trie.insert("FBCF", new AccumulatorValue());
    trie.insert("FBFD", new AccumulatorValue());
    trie.insert("FBFF", new AccumulatorValue());
    trie.insert("FFCD", new AccumulatorValue());
    trie.insert("FFCF", new AccumulatorValue());
    trie.insert("FFFD", new AccumulatorValue());
    trie.insert("FFFF", new AccumulatorValue());

    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABCD"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABCF"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABFD"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("ABFF"))->count == 3);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("AFCD"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("AFCF"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("AFFD"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("AFFF"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FBCD"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FBCF"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FBFD"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FBFF"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFCD"))->count == 3);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFCF"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFFD"))->count == 2);
    EXPECT_TRUE(((AccumulatorValue *) trie.lookup("FFFF"))->count == 2);
}


static char REVERSE_TLB[16] = {
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
};
string random_handle() {
    char buffer[HANDLE_HASH_SIZE];
    unsigned int key_size = HANDLE_HASH_SIZE - 1;
    for (unsigned int i = 0; i < key_size; i++) {
        buffer[i] = REVERSE_TLB[(rand() % 16)];
    }
    buffer[key_size] = 0;
    string s = buffer;
    return s;
}


void test5(){
    char buffer[1000];
    map<string, unsigned int> baseline;
    TestValue *value;

    for (unsigned int key_size: {2, 5, 10, 100}) {
        baseline.clear();
        HandleTrie *trie = new HandleTrie(key_size);
        for (unsigned int i = 0; i < 100000; i++) {
            for (unsigned int j = 0; j < key_size; j++) {
                buffer[j] = R_TLB[(rand() % 16)];
            }
            buffer[key_size] = 0;
            string s = buffer;
            if (baseline.find(s) == baseline.end()) {
                baseline[s] = 0;
            }
            baseline[s] = baseline[s] + 1;
            value = (TestValue *) trie->lookup(s);
            if (value == NULL) {
                value = new TestValue();
                trie->insert(s, value);
            } else {
                value->count += 1;
            }
        }
        for (auto const& pair : baseline) {
            value = (TestValue *) trie->lookup(pair.first);
            EXPECT_TRUE(value != NULL);
            EXPECT_EQ(pair.second, value->count);
        }
        delete trie;
    }
}


void test6(){
       char buffer[1000];
    map<string, int> baseline;
    TestValue *value;

    for (unsigned int key_count: {1, 2, 5}) {
        baseline.clear();
        unsigned int key_size = (HANDLE_HASH_SIZE - 1) * key_count;
        HandleTrie *trie = new HandleTrie(key_size);
        for (unsigned int i = 0; i < 100000; i++) {
            for (unsigned int j = 0; j < key_size; j++) {
                buffer[j] = R_TLB[(rand() % 16)];
            }
            buffer[key_size] = 0;
            string s = buffer;
            if (baseline.find(s) == baseline.end()) {
                baseline[s] = 0;
            }
            baseline[s] = baseline[s] + 1;
            value = (TestValue *) trie->lookup(s);
            if (value == NULL) {
                value = new TestValue();
                trie->insert(s, value);
            } else {
                value->count += 1;
            }
        }
        for (auto const& pair : baseline) {
            value = (TestValue *) trie->lookup(pair.first);
            EXPECT_EQ(pair.second, value->count);
        }
        delete trie;
    }
}

float* test7(unsigned int n_insertions = 1000000){
    char buffer[1000];
    static float return_values[4] = {0.0};
    // map<string, int> arr[3];
    map<string, int> baseline;
    // map<string, int> baseline1;
    // map<string, int> baseline2;

    TestValue *value;
    TestValue *value2;
    StopWatch timer_std;
    StopWatch timer_trie;
    StopWatch timer_std_none;
    StopWatch timer_trie_none;
    // 10000000

    // unsigned int n_insertions = 1000000;
    // unsigned int n_insertions = 10000000;


    // int count = -1;
    timer_std.start();
    for (unsigned int key_count: {1, 2, 5}) {
        // count++;
        // auto baseline = arr[count];
        unsigned int key_size = (HANDLE_HASH_SIZE - 1) * key_count;
        for (unsigned int i = 0; i < n_insertions; i++) {
            for (unsigned int j = 0; j < key_size; j++) {
                buffer[j] = R_TLB[(rand() % 16)];
            }
            buffer[key_size] = 0;
            string s = buffer;
            if (baseline.find(s) == baseline.end()) {
                baseline[s] = 0;
            }
            baseline[s] = baseline[s] + 1;
        }
    }
    timer_std.stop();

    // for(auto b: arr){
    //     b.clear();
    // }


    timer_std_none.start();
    for (unsigned int key_count: {1, 2, 5}) {
        unsigned int key_size = (HANDLE_HASH_SIZE - 1) * key_count;
        for (unsigned int i = 0; i < n_insertions; i++) {
            for (unsigned int j = 0; j < key_size; j++) {
                buffer[j] = R_TLB[(rand() % 16)];
            }
            buffer[key_size] = 0;
            string s = buffer;
        }
    }
    timer_std_none.stop();


    timer_trie.start();
    for (unsigned int key_count: {1, 2, 5}) {
        unsigned int key_size = (HANDLE_HASH_SIZE - 1) * key_count;
        HandleTrie *trie = new HandleTrie(key_size);
        for (unsigned int i = 0; i < n_insertions; i++) {
            for (unsigned int j = 0; j < key_size; j++) {
                buffer[j] = R_TLB[(rand() % 16)];
            }
            buffer[key_size] = 0;
            string s = buffer;
            value = (TestValue *) trie->lookup(s);
            if (value == NULL) {
                value = new TestValue();
                trie->insert(s, value);
            } else {
                value->count += 1;
            }
        }
        
    }
    timer_trie.stop();



    cout << "=======================================================" << endl;
    cout << "stdlib: " + timer_std.str_time() << endl;
    cout << "trie: " + timer_trie.str_time() << endl;

    cout << "stdlib access: " << (timer_std.milliseconds() - timer_std_none.milliseconds()) / 1000.0 << endl;
    cout << "trie access: " << (timer_trie.milliseconds() - timer_std_none.milliseconds()) / 1000.0 << endl;
    cout << "=======================================================" << endl;
    return_values[0] = timer_std.milliseconds() / 1000.;
    return_values[1] = timer_trie.milliseconds() / 1000.;
    return_values[2] = timer_std_none.milliseconds() / 1000;

    return return_values;
}


void producer(HandleTrie *trie, unsigned int n_insertions) {
    char buffer[1000];
    AccumulatorValue *value;
    unsigned int key_size = HANDLE_HASH_SIZE - 1;
    for (unsigned int i = 0; i < n_insertions; i++) {
        for (unsigned int j = 0; j < key_size; j++) {
            buffer[j] = R_TLB[(rand() % 16)];
        }
        buffer[key_size] = 0;
        string s = buffer;
        value = new AccumulatorValue();
        trie->insert(s, value);
    }
}

void visitor(HandleTrie *trie, unsigned int n_visits) {
    char buffer[1000];
    unsigned int key_size = HANDLE_HASH_SIZE - 1;
    for (unsigned int i = 0; i < n_visits; i++) {
        for (unsigned int j = 0; j < key_size; j++) {
            buffer[j] = R_TLB[(rand() % 16)];
        }
        buffer[key_size] = 0;
        string s = buffer;
        trie->lookup(s);
    }
}

void producer2(HandleTrie *trie, unsigned int n_insertions, string *handles) {
    AccumulatorValue *value;
    for (unsigned int i = 0; i < n_insertions; i++) {
        string s = handles[rand() % HANDLE_SPACE_SIZE];
        value = new AccumulatorValue();
        trie->insert(s, value);
    }
}

void visitor2(HandleTrie *trie, unsigned int n_visits, string *handles) {
    for (unsigned int i = 0; i < n_visits; i++) {
        string s = handles[rand() % HANDLE_SPACE_SIZE];
        trie->lookup(s);
    }
}

void test8(){
     vector<thread *> producers;
    vector<thread *> visitors;
    unsigned int n_insertions = 10000;
    unsigned int n_visits = 10000;
    StopWatch timer;
    timer.start();
    for (int n_producers: {2, 10, 100}) {
        for (int n_visitors: {2, 10, 100}) {
            unsigned int key_size = HANDLE_HASH_SIZE - 1;
            HandleTrie *trie = new HandleTrie(key_size);
            producers.clear();
            visitors.clear();
            for (int i = 0; i < n_producers; i++) {
                producers.push_back(new thread(&producer, trie, n_insertions));
            }
            for (int i = 0; i < n_visitors; i++) {
                visitors.push_back(new thread(&visitor, trie, n_visits));
            }
            for (thread *t: producers) {
                t->join();
            }
            for (thread *t: visitors) {
                t->join();
            }
            delete trie;
        }
    }
    timer.stop();
}

void test9(){
      string handles[HANDLE_SPACE_SIZE];
    for (unsigned int i = 0; i < HANDLE_SPACE_SIZE; i++) {
        handles[i] = random_handle();
    }
    vector<thread *> producers;
    vector<thread *> visitors;
    unsigned int n_insertions = 100000;
    unsigned int n_visits = 100000;
    for (int n_producers: {2, 10, 100}) {
        for (int n_visitors: {2, 10, 100}) {
            unsigned int key_size = HANDLE_HASH_SIZE - 1;
            HandleTrie *trie = new HandleTrie(key_size);
            for (int i = 0; i < n_producers; i++) {
                producers.push_back(new thread(&producer2, trie, n_insertions, handles));
            }
            for (int i = 0; i < n_visitors; i++) {
                visitors.push_back(new thread(&visitor2, trie, n_visits, handles));
            }
            for (thread *t: producers) {
                t->join();
            }
            for (thread *t: visitors) {
                t->join();
            }
            delete trie;
            producers.clear();
            visitors.clear();
        }
    }
}

int main(){


    // HandleTrie trie(3);
    // TestValue *value;

    // trie.insert("ABC", new TestValue(3));
    // trie.insert("ACB", new TestValue(4));
    // trie.insert("AAC", new TestValue(5));
    // trie.insert("BCD", new TestValue(6));

    // // value = (TestValue *) trie.lookup("ABCD");
    // // value = (TestValue *) trie.lookup("ABCF");
    // std::cout << trie.lookup("AAC")->to_string();

    // test1();
    // test2();
    // test3();
    // test5();
    // test6();
    // test7(100000);
    const int REPEATS = 3;
    int max = 1000000000;
    // const float repeats[REPEATS] = {0.0};
    map<string, vector<double>> m;
    // for (int i=1000; i< max; i*=5){
    for (unsigned int i: {1000, 10000, 100000, 1000000}){

        cout << "Testing " << i << " nodes" << endl;
        float sum_std = 0;
        float sum_trie = 0;
        float sum_none = 0;
        for(int j=0; j < REPEATS; j++){
            float* values = test7(i);
            m["std_map"].push_back(values[0]);
            m["trie"].push_back(values[1]);
            m["std_map_diff"].push_back(values[0] - values[2]);
            m["trie_diff"].push_back(values[1] - values[2]);

        }
        cout << "std_map = Average: " << accumulate(m["std_map"].begin(), m["std_map"].end(), 0.0) / REPEATS << " STDEV: " << variance(m["std_map"]) << endl;
        cout << "trie = Average: " << accumulate(m["trie"].begin(), m["trie"].end(), 0.0) / REPEATS << " STDEV: " << variance(m["trie"]) << endl;
        cout << "std_map diff = Average: " << accumulate(m["std_map_diff"].begin(), m["std_map_diff"].end(), 0.0) / REPEATS << " STDEV: " << variance(m["std_map_diff"]) << endl;
        cout << "trie diff = Average: " << accumulate(m["trie_diff"].begin(), m["trie_diff"].end(), 0.0) / REPEATS << " STDEV: " << variance(m["trie_diff"]) << endl;
        cout << "=======================================================" << endl;
       
        m.clear();
        cout << endl;
    }
    // test8();
    // test9();


    // for (int i=0; i < 10000000000; i++){
    //     std::string s =  random_handle();
    //     char * str = new char[s.length()];
    //     strcpy(str, s.c_str());

    //     auto a = compute_hash(str);
    //     auto b = compute_hash2(str);
    //     EXPECT_EQ(*a,*b);
    //     // cout << b << endl << a;



    // }


    // auto a = compute_hash("c");
    // auto b = compute_hash2("c");
    // EXPECT_EQ(*a,*b);
    // cout << b << endl << a;

    return 0;
}