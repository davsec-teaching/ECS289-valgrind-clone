#!/bin/bash

cd build; make -j4; cd ..

clang -c -emit-llvm -S -fpass-plugin=`echo build/skeleton/SkeletonPass.*` something.c -o something.ll

clang -c valgrind_clone.c -o valgrind_clone.o

clang something.ll valgrind_clone.o -o something

