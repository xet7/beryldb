## About

Beryl is a modular database written in C++. Beryl provides lists, maps, keys, and channel subscription for its clients. It is
optimized for GNU/Linux systems.

NOTE: Beryl is currently in beta and should **NOT** be used in a production
environment. At this time, BerylDB is only available for Linux systems.

## Installation

It is recommended to build Beryl from source. A guide on how to do this is available on [the Beryl docs site](https://docs.beryl.dev/installation/).
Beryl's source code is available in [our GitHub webpage](https://github.com/beryldb/beryldb).

NOTE: When running 'make', you should use the '-j4' argument, which allows you to compile 
using 4 cores. Feel free to change this number according to your available cpu.

You may install BerylDB following these quick steps:

```
./configure
make -j4 
make install
```

## Running

You can now start beryl by typing:

```
./run/beryldb start
```

Great! You may now run your very first BerylDB command using
[Beryldb-cli](https://github.com/beryldb/beryldb-cli) (also known as Emerald):

```
beryl> set hello "world"
beryl> OK
beryl> get hello
beryl> "world"
```

## External Links

* [Documentation](https://docs.beryl.dev)
* [GitHub](https://github.com/beryldb/beryldb)
* [Support/Discord](https://discord.gg/sqsXVYuGrX)

![Logo](https://docs.beryl.dev/img/logo.png)
