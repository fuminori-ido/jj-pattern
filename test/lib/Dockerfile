#= NAME
# Dockerfile  - for clean build sandbox
#
# = SYNOPSIS
# cd test/lib
# docker build --build-arg UID=$(id -u) \
#              --build-arg GID=$(id -g) \
#              -t jj-test \
#              .
# cd ../..
# docker run --rm -it --group-add sudo \
#     -v $(pwd):/home/user/jj-pattern.git jj-test bash

ARG BASE_IMAGE=ubuntu:24.04
FROM  $BASE_IMAGE

ARG GID
ARG UID
ARG USER=user

RUN apt-get update && \
    apt-get install -y sudo git cmake g++ && \
    groupadd --gid $GID $USER && \
    useradd -m --uid $UID --gid $GID -G sudo $USER

# enable user sudo(8)
COPY  10-installer /etc/sudoers.d/

USER  $UID:$GID
WORKDIR /home/$USER
