build-nanobind: clean
	@rm -f CMakeLists.txt handletrie_nanobind*.so
	@ln -s CMakeListsNanoBind.txt CMakeLists.txt
	@cd build && cmake .. && make && cp *.so ..

build-pybind: clean
	@rm -f CMakeLists.txt handletrie_pybind*.so
	@ln -s CMakeListsPyBind.txt CMakeLists.txt 
	@cd build && cmake .. && make && cp *.so ..


build-cpython: clean
	@rm -f handletrie_cpython*.so
	@python setup.py build_ext --inplace

build: clean
	@rm -f CMakeLists.txt
	@ln -s CMakeListsCXX.txt CMakeLists.txt
	@cd build && cmake .. && make

clean:
	@rm -rf build/*
	@rm -rf cpython/*
