#!/bin/bash
#
# = NAME
# build.sh  - build jj library user environment
#
# = SYNOPSIS
# 1. build image and run 'jj' container:
#     $ sample/build.sh
# 1. cd work
#     $ cd work
# 1. generate part-B (pattern definitions):
#     $ bgen 01_test.cpp >01_test.b   # generate part-B file.
# 1. compile:
#     $ g++ 01_test.cpp -ljj -lgtest -lgtest_main -pthread
#
# = DESCRIPTION

dir=$(pwd)

#docker build -t jj:0.0.1-cpp -f sample/Dockerfile-cpp .
docker build -t jj:0.0.1-lib -f sample/Dockerfile-lib .
docker build -t jj:0.0.1     -f sample/Dockerfile     .
docker run --rm -it \
    --name      jj \
    --hostname  jj \
    -v          $dir/test/pattern:/home/yariscross/work \
    jj:0.0.1 bash
