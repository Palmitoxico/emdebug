# EmDebug

[![Build Status](https://travis-ci.org/Palmitoxico/emdebug.svg?branch=master)](https://travis-ci.org/Palmitoxico/emdebug)

## Description

EmDebug is a front-end interface for GDB + OpenOCD, designed for debug ARM Cortex-M based microcontrollers. GDB is ok when debugging most of the code logic, but isn't much convenient when you need to inspect peripheral's registers, GPIO or what execution mode is the current.

Planned features:

* Inspect and modify peripheral's registers;
* Visualize the GPIO status;
* Show information about the current execution mode (privileged/unprivileged);
* Show Stack Frame.

## Status

The development stills a little slow, up to now the gdbmi library isn't ready (the MI interface isn't trivial to implement, specially when dealing with asynchronous events), nor the graphical interface, but something functional is expected in the end of 2016.

## How to build

Pre-requisites:

* cmake (3.1.0 or later)
* libwxgtk3.0-dev
* g++ (with c++11 support)
* make

Instructions:

Make a build directory then use cmake to generate the makefile:

```
emdebug$ mkdir build
emdebug$ cd build
build$ cmake ..
build$ make
```
