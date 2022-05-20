===============================
README
===============================

1. Compiling the C++ code 
2. Running the C++ unittests
3. Running the C++ program

## 1. Compiling the C++ code 

Getting cmake to generate a build.ninja file (instead of a Makefile)
--------------------------------------------------------------------

To have cmake generate ninja.build files instead of Makefiles, do this:
(it's more usual to build into a subdir called build with cmake)

mkdir -p build
cd build
cmake -GNinja ..
ninja

Note: ninja does the actual compiling (and linking)

## 2. Running the C++ unittests

cd build
./test_main

## 3. Running the program

cd build
./main

