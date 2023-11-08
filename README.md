# Replay Clocks

This project designs a new version of the vector clock based on Hybrid Logical Clocks. We extend the idea of HLC to devise a vector clock that deals with offsets instead of the traditional long form timestamps for all processes in the system. 

## Usage

The python version of the RepCL is available under the python filetree, and the C++ source can be built from the c++ filetree. Both versions are self contained and necessary imports are listed at the top of each file. Both versions should work with any standard installation of python/C++.

Corresponding unit testcase files are also available. To use the RepCL, just import the necessary file and use it as a traditional module to replace the system clock. Be sure to implement all the clock functionality you require on top of the existing implementation. For more information, refer to our paper.