# = NAME
# Dockerfile  - build jj:[JJ_VER]
#
# = SYNOPSIS
# docker build -t jj:0.0.1 -f sample/Dockerfile .
# docker run --rm -it \
#     --name      jj \
#     --hostname  jj \
#     jj:0.0.1 bash
#
# = DESCRIPTION
# build jj:[JJ_VER] from jj:[JJ_VER]-cpp
#
# jj:[JJ_VER] is for testing 'jj' library in isolated environment.
#
# 1. install git & cmake to build googletest
#
# == Docker image tags
# jj:[JJ_VER]-cpp::   g++ compiler environment
# jj:[JJ_VER]-lib::   jj/pattern library is installed
# jj:[JJ_VER]::       jj testing environment
# 

FROM jj:0.0.1-lib

# change to your name
ARG user="yariscross"

WORKDIR /tmp/googletest-build

RUN apt-get install -y vim git cmake && \
    git clone https://github.com/google/googletest.git -b v1.13.0 && \
    cd googletest/ && \
    mkdir build && \
    cd build/ && \
    cmake .. && \
    make && \
    make install

USER $user
WORKDIR /home/$user
