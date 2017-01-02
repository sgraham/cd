#!/bin/sh
# export LD_LIBRARY_PATH=`pwd`/third_party/lib
# ulimit -c unlimited
clang++ -g crash.cc -o crash && ./crash; clang++ -std=c++11 -Wall -Werror -g -Ithird_party/include/lldb/API -Ithird_party/include main.cc -llldb -Lthird_party/lib -o cd && ./cd crash
