# setup.py
from setuptools import setup, Extension
import os
import sys
from os import listdir
from os.path import isfile, join
onlyfiles = [f"src/{f}" for f in listdir('src/') if isfile(join('src/', f)) and '.h' not in f and 'main' not in f and 'pybind' not in f] 
print(onlyfiles)

openssl_include_dir = '/usr/include/openssl/include'
openssl_lib_dir = '/usr/include/openssl/lib'

# Define the extension module
extension = Extension(
    'handletrie',
    sources=onlyfiles,
    # sources=['src/cpython.cc'],
    # include_dirs=['src'],
    # extra_compile_args=['-std=c++11'],
    # # extra_link_args=['-lcrypto'],
    #  libraries=['ssl', 'crypto']
    include_dirs=[openssl_include_dir, 'src'],
    library_dirs=[openssl_lib_dir],
    libraries=['ssl', 'crypto'],  # Link against the OpenSSL libraries
    extra_compile_args=['-std=c++11', '-O3'],
    extra_link_args=['-lcrypto']
)

# Setup configuration
setup(
    name='handletrie',
    version='0.1',
    ext_modules=[extension],
)
