#include <Python.h>
#include "HandleTrie.h"
#include <iostream>

using namespace attention_broker_server;
using namespace std;

// Wrapper class to hold an instance
typedef struct {
    PyObject_HEAD
    HandleTrie* trie;
} PyHandleTrieObject;

// template<typename T>
class Value: public HandleTrie::TrieValue {
    public:
        PyObject*  value_;
        Value(PyObject* value) {
            this->value_ = value;
            // Py_XINCREF(value_);
        }
        void merge(TrieValue *other) {

        }
        ~Value() {
            delete value_;
            // Py_XDECREF(value_);
        }

        std::string to_string(){
        }
};



// Method to initialize a new HandleTrie object
static int PyHandleTrie_init(PyHandleTrieObject* self, PyObject* args) {
    int key_size;
    if (!PyArg_ParseTuple(args, "I", &key_size)) {
        return -1;
    }
    self->trie = new HandleTrie(key_size);
    return 0;
}


static PyObject* PyHandleTrie_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
    PyHandleTrieObject* self;
    self = (PyHandleTrieObject*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->trie = NULL;  // Initialize the point member to NULL
    }
    return (PyObject*)self;
}

// Method to insert
static PyObject* PyHandleTrie_insert(PyHandleTrieObject* self, PyObject* args) {
    PyObject* value;
    // string key;
    const char* key;
    if (!PyArg_ParseTuple(args, "sO", &key, &value)) {
        return NULL;
    }
    string s = key;
    self->trie->insert(s, new Value(value));
    Py_RETURN_NONE;
}


static PyObject* PyHandleTrie_lookup(PyHandleTrieObject* self, PyObject* args) {
    const char* key;
    if (!PyArg_ParseTuple(args, "s", &key)) {
        return NULL;
    }
    string k = key;
    Value* value = (Value *) self->trie->lookup(k);
    if (value == NULL){
        Py_RETURN_NONE;
    }
    return value->value_;
}


// Define the methods of the PyHandleTrie class
static PyMethodDef PyHandleTrie_methods[] = {
    {"insert", (PyCFunction)PyHandleTrie_insert, METH_VARARGS, ""},
    {"lookup", (PyCFunction)PyHandleTrie_lookup, METH_VARARGS, ""},
    {NULL}  // Sentinel
};

// Define the PyHandleTrie type
static PyTypeObject PyHandleTrieType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "handletrie_cpython.HandleTrie",          // tp_name
    sizeof(PyHandleTrieObject),        // tp_basicsize
    0,                            // tp_itemsize
    NULL,  // tp_dealloc
    0,                            // tp_vectorcall_offset
    NULL,                         // tp_getattr
    NULL,                         // tp_setattr
    NULL,                         // tp_as_async
    NULL,        // tp_repr
    NULL,                         // tp_as_number
    NULL,                         // tp_as_sequence
    NULL,                         // tp_as_mapping
    NULL,        // tp_hash
    NULL,                         // tp_call
    NULL,         // tp_str
    NULL,                         // tp_getattro
    NULL,                         // tp_setattro
    NULL,                         // tp_as_buffer
    NULL,            // tp_flags
    "HandleTrie objects",              // tp_doc
    NULL,                         // tp_traverse
    NULL,                         // tp_clear
    NULL,                         // tp_richcompare
    0,                            // tp_weaklistoffset
    NULL,                         // tp_iter
    NULL,                         // tp_iternext
    PyHandleTrie_methods,               // tp_methods
    NULL,                         // tp_members
    NULL,                         // tp_getset
    NULL,                         // tp_base
    NULL,                         // tp_dict
    NULL,                         // tp_descr_get
    NULL,                         // tp_descr_set
    0,                            // tp_dictoffset
    (initproc)PyHandleTrie_init,        // tp_init
    NULL,                         // tp_alloc
    PyHandleTrie_new,                   // tp_new
};

// Initialize the module
static PyModuleDef handletrie = {
    PyModuleDef_HEAD_INIT,
    "handletrie_cpython",                // m_name
    "Example module that creates a HandleTrie", // m_doc
    -1,                           // m_size
    NULL,                         // m_methods
};

// Initialize the module
PyMODINIT_FUNC PyInit_handletrie_cpython(void) {
    PyObject* m;
    if (PyType_Ready(&PyHandleTrieType) < 0) {
        return NULL;
    }
    m = PyModule_Create(&handletrie);
    if (m == NULL) {
        return NULL;
    }
    Py_INCREF(&PyHandleTrieType);
    PyModule_AddObject(m, "HandleTrie", (PyObject*)&PyHandleTrieType);
    return m;
}
