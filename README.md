# NGServer

A Light cpp game server.

test on ubuntu-16.04-lts.

## Prepare

### Install clang

```
$ sudo apt install clang
```

### Install openssl library

```
$ sudo apt install openssl libssl-dev
```

### Install uuid library

```
$ sudo apt-get install uuid-dev
```

### Install Mysql Server

```
$ sudo apt-get install mysql-server
```

### Install mysql-cpp-connector

```
$ sudo apt install libmysqlcppconn-dev
```

### Install glog

```
$ git clone https://github.com/google/glog.git
$ sudo apt-get install autoconf automake libtool
$ cd glog
$ ./autogen.sh
$ ./configure
$ make -j 8
$ sudo make install
```

### Install libuv

```
$ git clone https://github.com/libuv/libuv.git
$ cd libuv
$ ./autogen.sh
$ ./configure
$ make
$ make check
$ sudo make install
```

### Install protobuf

```
$ git clone https://github.com/google/protobuf.git
$ sudo apt-get install autoconf automake libtool curl make g++ unzip
$ cd protobuf/
$ ./autogen.sh
$ ./configure --prefix=/usr/local/lib/protobuf
$ make
$ make check
$ sudo make install
```

### Install cmake

```
$ sudo apt install cmake
```

### Install gtest

```
$ git clone https://github.com/google/googletest.git
$ cd googletest
$ cmake .
$ make
$ sudo make install
```

## Build

```
$ cmake .
$ make
$ ./src/Server/Server
```

If success, the terminal will show the log like below.

```
I0307 17:21:16.692776  8822 _Server.cpp:241] server start, pid: 8822, tid: 139948436178816
I0307 17:21:16.760125  8822 _Server.cpp:247] connect database success
I0307 17:21:16.760208  8822 _Server.cpp:103] ListenServer, IP: 0.0.0.0 Port: 7000
I0307 17:21:16.760246  8822 _Server.cpp:108] KeepAlive: 1
I0307 17:21:16.760373  8822 _Server.cpp:202] TimerHanlde 50ms NotifyClient
```

