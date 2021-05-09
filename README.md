This README is a brief introduction to Beryl. For extended information, you
can visit our full documentation site at [beryl.dev](https://docs.beryl.dev/).

## What is Beryl?

Beryl is a data structures server. This basically means that Beryl provides
access to structures via a wide range of commands. Processes are thus queued
and dispatched to an internal processor.

Beryl provides lists, maps, keys, and channel subscription for its clients. It is
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

## Running Beryl

To run Beryl with default configuration, just type:

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

## Contributing

We are always welcoming new members. If you wish to start contributing code to the 
Beryl project in any form, such as in the form of pull requests via Github, 
a code snippet or a patch, you would need to agree to release your work under the terms of the
BSD license.

## External Links

* [Documentation](https://docs.beryl.dev)
* [GitHub](https://github.com/beryldb/beryldb)
* [Support/Discord](https://discord.gg/sqsXVYuGrX)

![Logo](https://docs.beryl.dev/img/smaller.png)
