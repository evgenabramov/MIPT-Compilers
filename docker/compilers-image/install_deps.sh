#!/bin/sh

set -e -x

apt-get update

apt-get install -y \
	ssh \
	make \
	cmake \
	ninja-build \
	git \
	clang-8 \
	clang-format-8 \
	clang-tidy-8 \
	python3 \
	python3-pip \
	python3-venv \
	ca-certificates \
	openssh-server \
	rsync \
	lldb-8 \
	vim \
	gdb \
	libboost-all-dev \
	wget \
	autoconf \
    tree \
    bison \
    flex \
    tar \
    g++ \
    arm-linux-gnueabihf \
    libc6-dev-armhf-cross \
    qemu-user-static

pip3 install \
	click \
	gitpython \
	python-gitlab \
	termcolor \
	virtualenv

