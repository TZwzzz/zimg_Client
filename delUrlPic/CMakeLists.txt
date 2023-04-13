cmake_minimum_required(VERSION 2.6)
#创建工程
project(delUrlPic)
#C++版本为11
set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(delUrlPic main.cpp) #将程序生成可执行文件
target_link_libraries(delUrlPic curl) #链接lib文件到可执行文件中