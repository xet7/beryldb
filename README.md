
<a target="_blank" href="https://twitter.com/beryldb"><img src="https://img.shields.io/twitter/url/https/twitter.com/cloudposse.svg?style=social&label=Follow%20%40beryldb"></a>
<a target="_blank" href="https://github.com/beryldb/beryldb/actions"><img src="https://github.com/beryldb/beryldb/workflows/Linux%20build/badge.svg?4"></a>
<a target="_blank" href="https://github.com/beryldb/beryldb/pulse" alt="Activity"> <img src="https://img.shields.io/github/commit-activity/m/beryldb/beryldb?5" /></a>
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
<br>


This README is a brief introduction to Beryl. For extended information, you
can visit our full documentation site at [beryl.dev](https://docs.beryl.dev/).

**Note:** This is an experimental/learning project and is not intended to be used in
a production setting.

![Logo](https://docs.beryl.dev/img/smaller.png??)

## What is Beryl?

Beryl is a data structures server. This basically means that Beryl provides
access to structures via a wide range of commands. Processes are thus queued
and dispatched to an internal processor.

Beryl provides lists, maps, keys, and channel subscription for its clients. It is
optimized for GNU/Linux systems.

Don't forget to follow us on [Twitter](https://twitter.com/beryldb)

![cli](https://docs.beryl.dev/img/render.gif??)

## Installation

It is recommended to build Beryl from source. A guide on how to do this is available on [the Beryl docs site](https://docs.beryl.dev/server/installation/).
Beryl's source code is available in [our GitHub webpage](https://github.com/beryldb/beryldb).

NOTE: When running 'make', you should use the '-j4' argument, which allows you to compile 
using 4 cores. Feel free to change this number according to your available cpu.

Install dependencies:

```
sudo apt-get install librocksdb-dev build-essential
```

Download latest version:

```
wget https://github.com/beryldb/beryldb/archive/refs/tags/0.0.1.tar.gz
tar -xvzf 0.0.1.tar.gz
cd beryldb-0.0.1/
```

You may now configure BerylDB following these quick steps: 

```
./configure
make -j4 
make install
```

## Running Beryl

To run Beryl with default configuration, type:

```
./run/beryldb start
```

Great! You may now run your very first BerylDB command using
[Beryldb-cli](https://github.com/beryldb/beryldb-cli) (also known as Emerald):


## Contributing

We are always welcoming new members. If you wish to start contributing code to the 
Beryl project in any form, such as in the form of pull requests via Github, 
a code snippet or a patch, you would need to agree to release your work under the terms of the
BSD license.

## External Links

* [Documentation](https://docs.beryl.dev)
* [GitHub](https://github.com/beryldb/beryldb)
* [Support/Discord](https://discord.gg/sqsXVYuGrX)
* [Twitter](https://twitter.com/beryldb)


