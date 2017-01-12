# GDB
This tutorial was originally written by [Andrew Gilpin](http://www.cs.cmu.edu/~gilpin/).

This lab is prepared by [LI Haocheng](https://rholais.github.io/csci3150/gdb/).

The purpose of a debugger such as gdb is to allow you to see what is going on “inside” another program while it executes - or what another program was doing at the moment it crashed.

gdb can do four main kinds of things (plus other things in support of these) to help you catch bugs in the act:

* Start your program, specifying anything that might affect its behavior.
* Make your program stop on specified conditions.
* Examine what has happened, when your program has stopped.
* Change things in your program, so you can experiment with correcting the effects of one bug and go on to learn about another.

##  Supported Languages
gdb supports C, C++, D, Go, Objective-C, Fortran, OpenCL C, Pascal, Rust, assembly, Modula-2, and Ada. Some gdb features may be used in expressions regardless of the language you use: the gdb `@` and `::` operators, and the `{type}addr` construct can be used with the constructs of any supported language.

##  Expressions
`print` and many other gdb commands accept an expression and compute its value. Any kind of constant, variable or operator defined by the programming language you are using is valid in an expression in gdb. This includes conditional expressions, function calls, casts, and string constants. It also includes preprocessor macros, if you compiled your program to include this information; see [Compilation](https://sourceware.org/gdb/current/onlinedocs/gdb/Compilation.html).

##  Source Code
To help illustrate some of the debugging principles I will use a running example of a buggy program. As you progress through this tutorial, you will use the debugger to locate and fix errors in the code. The code can be downloaded [here](src/main.c) and a simple Makefile for the program can be downloaded [here](src/Makefile).

The code is very simple and consists of two class definitions, a node and a linked list. There is also a simple driver to test the list. All of the code was placed into a single file to make illustrating the process of debugging a little easier.
