build-nanobind: clean
	@rm -f CMakeLists.txt lib/handletrie_nanobind*.so
	@ln -s CMakeListsNanoBind.txt CMakeLists.txt
	@cd build && cmake .. && make && cp *.so ../lib/

build-pybind: clean
	@rm -f CMakeLists.txt lib/handletrie_pybind*.so
	@ln -s CMakeListsPyBind.txt CMakeLists.txt 
	@cd build && cmake .. && make && cp *.so ../lib/


build-cpython: clean
	@rm -f lib/handletrie_cpython*.so
	@python setup.py build_ext --inplace && mv *.so lib/

build: clean
	@rm -f CMakeLists.txt
	@ln -s CMakeListsCXX.txt CMakeLists.txt
	@cd build && cmake .. && make

clean:
	@rm -rf build/*
	@rm -rf cpython/*
