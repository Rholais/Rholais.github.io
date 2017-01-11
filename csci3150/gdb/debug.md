# Debugging

##  When to use a debugger

Debugging is something that can't be avoided. Every programmer will at one point in their programming career have to debug a section of code. There are many ways to go about debugging, from printing out messages to the screen, using a debugger, or just thinking about what the program is doing and making an educated guess as to what the problem is.
Before a bug can be fixed, the source of the bug must be located. For example, with segmentation faults, it is useful to know on which line of code the seg fault is occuring. Once the line of code in question has been found, it is useful to know about the values in that method, who called the method, and why (specifically) the error is occuring. Using a debugger makes finding all of this information very simple.

Go ahead and make the program for this tutorial, and run the program. The program will print out some messages, and then it will print that it has received a segmentation fault signal, resulting in a program crash. Given the information on the screen at this point, it is near impossible to determine why the program crashed, much less how to fix the problem. We will now begin to debug this program.

##  Loading a program

So you now have an executable file (in this case main) and you want to debug it. First you must launch the debugger. The debugger is called gdb and you can tell it which file to debug at the shell prompt. So to debug main we want to type gdb main. Here is what it looks like when I run it:

    hcli@mint-VirtualBox ~/Projects/Rholais.github.io/csci3150/gdb/src $ gdb main
    GNU gdb (Ubuntu 7.11.1-0ubuntu1~16.04) 7.11.1
    Copyright (C) 2016 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
    and "show warranty" for details.
    This GDB was configured as "x86_64-linux-gnu".
    Type "show configuration" for configuration details.
    For bug reporting instructions, please see:
    <http://www.gnu.org/software/gdb/bugs/>.
    Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.
    For help, type "help".
    Type "apropos word" to search for commands related to "word"...
    Reading symbols from main...done.
    (gdb) 

gdb is now waitng for the user to type a command. We need to run the program so that the debugger can help us see what happens when the program crashes. Type `run` or `r` at the (gdb) prompt. Here is what happens when I run this command:

    (gdb) r
    Starting program: /home/hcli/Projects/Rholais.github.io/csci3150/gdb/src/main 
    Creating Node, 1 are in existence right now
    Creating Node, 2 are in existence right now
    Creating Node, 3 are in existence right now
    Creating Node, 4 are in existence right now
    The fully created list is:
    4
    3
    2
    1

    Now removing elements:
    Creating Node, 5 are in existence right now
    Destroying Node, 4 are in existence right now
    4
    3
    2
    1


    Program received signal SIGSEGV, Segmentation fault.
    0x0000000000401014 in Node<int>::next (this=0x0) at main.cpp:34
    34		Node<T>* next() const { return next_; }
    (gdb)

The program crashed so lets see what kind of information we can gather.

##  Inspecting crashes

So already we can see the that the program was at line 34 of main.cpp, that this points to `0x0`, and we can see the line of code that was executed. But we also want to know who called this method and we would like to be able to examine values in the calling methods. So at the `gdb` prompt, we type `backtrace` or `bt` which gives me the following output:

    (gdb) bt
    #0  0x0000000000401014 in Node<int>::next (this=0x0) at main.cpp:34
    #1  0x0000000000400f2e in LinkedList<int>::remove (this=0x614c20, item_to_remove=@0x7fffffffddfc: 1) at main.cpp:83
    #2  0x0000000000400bd6 in main (argc=1, argv=0x7fffffffdf08) at main.cpp:126
    (gdb) 

So in addition to what we knew about the current method and the local variables, we can now also see what methods called us and what their parameters were. For example, we can see that we were called by LinkedList<int>::remove () where the parameter `item_to_remove` is at address `0xffbef014`. It may help us to understand our bug if we know the value of `item_to_remove`, so we want to see the value at the address of `item_to_remove`. This can be done using the `frame` or `f` command using the stack frame number or the address of the frame as a parameter to select and print a stack frame. Then use the `print EXP` or `p EXP` command Print value of expression `EXP`. Here is what happens when I run the command:

    (gdb) f 1
    #1  0x0000000000400f2e in LinkedList<int>::remove (this=0x614c20, item_to_remove=@0x7fffffffddfc: 1) at main.cpp:83
    83				marker = marker->next();
    (gdb) p item_to_remove
    $2 = (const int &) @0x7fffffffddfc: 1
    (gdb) 

So the program is crashing while trying to run LinkedList<int>::remove with a parameter of 1. We have now narrowed the problem down to a specific function and a specific value for the parameter.


