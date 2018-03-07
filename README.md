# NGServer

A Light cpp game server.

test on ubuntu-16.04-lts.

## Prepare

### Install clang

$ sudo apt install clang

### Install openssl library

$ sudo apt install openssl libssl-dev

### Install uuid library

$ sudo apt-get install uuid-dev

### Install Mysql Server

$ sudo apt-get install mysql-server

### Install mysql-cpp-connector

$ sudo apt install libmysqlcppconn-dev

### Install glog

$ git clone https://github.com/google/glog.git
$ sudo apt-get install autoconf automake libtool
$ cd glog
$ ./autogen.sh
$ ./configure
$ make -j 8
$ sudo make install

### Install libuv

$ git clone https://github.com/libuv/libuv.git
$ cd libuv
$ ./autogen.sh
$ ./configure
$ make
$ make check
$ sudo make install

### Install protobuf
$ git clone https://github.com/google/protobuf.git
$ sudo apt-get install autoconf automake libtool curl make g++ unzip
$ cd protobuf/
$ ./autogen.sh
$ ./configure --prefix=/usr/local/lib/protobuf
$ make
$ make check
$ sudo make install

### Install cmake
$ sudo apt install cmake

### Install gtest
$ git clone https://github.com/google/googletest.git
$ cd googletest
$ cmake .
$ make
$ sudo make install

## Build


