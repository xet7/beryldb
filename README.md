
<a target="_blank" href="https://twitter.com/beryldb"><img src="https://img.shields.io/twitter/url/https/twitter.com/cloudposse.svg?style=social&label=Follow%20%40beryldb"></a>
<a target="_blank" href="https://github.com/beryldb/beryldb/actions"><img src="https://github.com/beryldb/beryldb/workflows/Linux%20build/badge.svg?4"></a>
<a target="_blank" href="https://github.com/beryldb/beryldb/pulse" alt="Activity"> <img src="https://img.shields.io/github/commit-activity/m/beryldb/beryldb?5" /></a>
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
<br>


This README is a brief introduction to Beryl. For extended information, you
can visit our documentation site at [beryl.dev](https://docs.beryl.dev/).

**Note:** This is an experimental/learning project and is not intended to be used in
a production setting.

![Logo](https://docs.beryl.dev/img/smaller.png??)

## What is Beryl?

Beryl is a data structures server. Our software provides access to structures via a 
wide range of commands, which are later queued, processed and dispatched to an 
internal processor. Beryl is written in C++ and is powered by [RocksDB](https://github.com/facebook/rocksdb).


Beryl provides lists, maps, keys, and channel subscription for its clients. It is
optimized for GNU/Linux systems.

Don't forget to follow us on [Twitter](https://twitter.com/beryldb)

![cli](https://docs.beryl.dev/img/render.gif??)

## Installation

It is recommended to build Beryl from source. A guide on how to do this is available on [the Beryl docs site](https://docs.beryl.dev/server/installation/).
Beryl's development branches are available in [our GitHub repository](https://github.com/beryldb/beryldb).

NOTE: When running 'make', you should use the '-j4' argument, which allows you to compile 
using 4 cores. Feel free to change this number according to your cpu.

Install dependencies:

```
sudo apt-get install librocksdb-dev build-essential
```

Download latest version:

```
wget https://github.com/beryldb/beryldb/archive/refs/tags/0.2.1.tar.gz
tar -xvzf 0.2.1.tar.gz
cd beryldb-0.2.1/
```

You may now configure BerylDB following these quick steps: 

```
./configure
make -j4 
make install
```

Beryl is compiled in verbose level by default. If you prefer to build with minimal
information, you can use:

```
make BERYLDB_VERBOSE=0
```

## Running Beryl

To run Beryl with default configuration, type:

```
./run/beryldb start
```

**NOTE**: Beryl runs in the background by default. If you wish to avoid
forking, you can run the server using nofork:

```
./run/beryldb start --nofork
```

Great! You may now run your very first BerylDB command using
[Beryl-cli](https://github.com/beryldb/beryldb-cli).

## Internals

In order to maintain a documentation that is easy to follow, we try to maintain 
discussions at a very shallow level. Our code is actively changing and thus
a brief discussion board is always appreacited.

## Source code organization

Beryl's source code is contained in the `src/` directory. The Makefile, which is
created after configuring Beryl, is located in the root directory.

Inside the root directory, you will find:

* `src`: Contains Beryl's implementation, written in C++.
* `include`: Contains Beryl's headers.
* `etc`: Libraries developed by 3rd party organizations.
* `make`: Contains several Perl-based functions to compile BerylDB.

## Contributing

We are always welcoming new members. If you wish to start contributing code to the 
Beryl project in any form, such as in the form of pull requests via Github, 
a code snippet or a patch, you will need to agree to release your work under the terms of the
BSD license.

## External Links

* **IRC**: irc.libera.chat - #beryl
* [Documentation](https://docs.beryl.dev)
* [GitHub](https://github.com/beryldb/beryldb)
* [Support/Discord](https://discord.gg/sqsXVYuGrX)
* [Twitter](https://twitter.com/beryldb)


