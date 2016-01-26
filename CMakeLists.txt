cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-O2")        ## Optimize
project( NeuralNet )
find_package( OpenCV REQUIRED )
add_executable( Ann main.cpp Gene.cpp InputNeuron.cpp Manager.cpp Network.cpp Neuron.cpp Prompt.cpp )
add_definitions(-std=c++11)
target_link_libraries( Ann ${OpenCV_LIBS} )
