#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "HandleTrie.h"


using namespace attention_broker_server;
using namespace std;
namespace py = pybind11;

class Value : public HandleTrie::TrieValue {
    public:
        pybind11::object value_;
        Value(pybind11::object* value) {
            this->value_ = value;
        }
        void merge(TrieValue *other) {

        }


        pybind11::object get_value(){
            return this->value_;
        }

        std::string to_string(){
        }
};




PYBIND11_MODULE(handletrie, m) {


    py::class_<Value>(m, "Value")
        .def(py::init<T>());


    py::class_<HandleTrie>(m, "HandleTrie")
        .def(py::init<int>())
        .def("insert", [](HandleTrie& self, py::ref key, py::object obj){
            Value *v = new Value(obj);
            std::string k = key.cast<std::string>();
            self.insert(k, v);
        })
        .def("lookup", &HandleTrie::lookup);
}