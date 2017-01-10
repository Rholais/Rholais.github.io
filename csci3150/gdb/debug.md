# Debugging

##  When to use a debugger

Debugging is something that can't be avoided. Every programmer will at one point in their programming career have to debug a section of code. There are many ways to go about debugging, from printing out messages to the screen, using a debugger, or just thinking about what the program is doing and making an educated guess as to what the problem is.
Before a bug can be fixed, the source of the bug must be located. For example, with segmentation faults, it is useful to know on which line of code the seg fault is occuring. Once the line of code in question has been found, it is useful to know about the values in that method, who called the method, and why (specifically) the error is occuring. Using a debugger makes finding all of this information very simple.

Go ahead and make the program for this tutorial, and run the program. The program will print out some messages, and then it will print that it has received a segmentation fault signal, resulting in a program crash. Given the information on the screen at this point, it is near impossible to determine why the program crashed, much less how to fix the problem. We will now begin to debug this program.

##  Loading a program

So you now have an executable file (in this case main) and you want to debug it. First you must launch the debugger. The debugger is called gdb and you can tell it which file to debug at the shell prompt. So to debug main we want to type gdb main. Here is what it looks like when I run it:

    agg1@sukhoi agg1/.www-docs/tutorial> gdb main
    GNU gdb 4.18
    Copyright 1998 Free Software Foundation, Inc.
    GDB is free software, covered by the GNU General Public License, and you are
    welcome to change it and/or distribute copies of it under certain conditions.
    Type "show copying" to see the conditions.
    There is absolutely no warranty for GDB.  Type "show warranty" for details.
    This GDB was configured as "sparc-sun-solaris2.7"...
    (gdb)

