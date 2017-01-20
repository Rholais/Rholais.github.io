# Debugging

##  When to Use a Debugger

Debugging is something that can't be avoided. Every programmer will at one point in their programming career have to debug a section of code. There are many ways to go about debugging, from printing out messages to the screen, using a debugger, or just thinking about what the program is doing and making an educated guess as to what the problem is.

Before a bug can be fixed, the source of the bug must be located. For example, with segmentation faults, it is useful to know on which line of code the seg fault is occuring. Once the line of code in question has been found, it is useful to know about the values in that method, who called the method, and why (specifically) the error is occuring. Using a debugger makes finding all of this information very simple.

Go ahead and make the program for this tutorial, and run the program. The program will print out some messages, and then it will print that it has received a segmentation fault signal, resulting in a program crash. Given the information on the screen at this point, it is near impossible to determine why the program crashed, much less how to fix the problem. We will now begin to debug this program.

##  Loading a Program

So you now have an executable file (in this case main) and you want to debug it. First you must launch the debugger. The debugger is called gdb and you can tell it which file to debug at the shell prompt. So to debug main we want to type gdb main. Here is what it looks like when I run it:

    hcli@mint-VirtualBox ~/Projects/Rholais.github.io/csci3150/gdb/src $ gdb segfault
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
    Reading symbols from segfault...done.
    (gdb) 

gdb is now waitng for the user to type a command. We need to run the program so that the debugger can help us see what happens when the program crashes. Type `run` or `r` at the (gdb) prompt. Here is what happens when I run this command:

    (gdb) r
    Starting program: /home/hcli/Projects/Rholais.github.io/csci3150/gdb/src/segfault 


Type in a string to continue running, for example `csci3150`:

    csci3150

    Program received signal SIGSEGV, Segmentation fault.
    __GI__IO_getline_info (fp=fp@entry=0x7ffff7dd18e0 <_IO_2_1_stdin_>, buf=buf@entry=0x0, n=1022, delim=delim@entry=10, extract_delim=extract_delim@entry=1, eof=eof@entry=0x0) at iogetline.c:77
    77	iogetline.c: No such file or directory.
    (gdb) 

The program crashed so lets see what kind of information we can gather.

##  Inspecting Crashes

So already we can see the that the program was at line 75 of main.cpp and we can see the line of code that was executed. But we also want to know who called this method and we would like to be able to examine values in the calling methods. So at the `gdb` prompt, we type `backtrace` or `bt` which gives me the following output:

    (gdb) bt
    #0  __GI__IO_getline_info (fp=fp@entry=0x7ffff7dd18e0 <_IO_2_1_stdin_>, buf=buf@entry=0x0, n=1022, delim=delim@entry=10, extract_delim=extract_delim@entry=1, eof=eof@entry=0x0) at iogetline.c:77
    #1  0x00007ffff7a7cd78 in __GI__IO_getline (fp=fp@entry=0x7ffff7dd18e0 <_IO_2_1_stdin_>, buf=buf@entry=0x0, n=<optimized out>, delim=delim@entry=10, extract_delim=extract_delim@entry=1) at iogetline.c:34
    #2  0x00007ffff7a7bb7d in _IO_fgets (buf=0x0, n=<optimized out>, fp=0x7ffff7dd18e0 <_IO_2_1_stdin_>) at iofgets.c:53
    #3  0x0000000000400675 in main (argc=1, argv=0x7fffffffde48) at segfault.c:10
    (gdb) 

We can use the `frame` or `f` command using the stack frame number or the address of the frame as a parameter to select and print a stack frame and use the `print EXP` or `p EXP` command Print value of expression `EXP`. Here is what happens when I run the command:

    (gdb) f 3
    #3  0x0000000000400675 in main (argc=1, argv=0x7fffffffde48) at segfault.c:10
    10	    fgets(buf, 1024, stdin);
    (gdb) p buf
    $1 = 0x0
    (gdb) 


So the program is crashing while trying to run `fgets` with a parameter of `NULL`. We have now narrowed the problem down to a specific function and a specific value for the parameter.

##  Conditional Breakpoints

Now that we know where and when the segfault is occuring, we want to watch what the program is doing right before it crashes. One way to do this is to step through, one at a time, every statement of the program until we get to the point of execution where we want to see what is happening. This works, but sometimes you may want to just run to a particular section of code and stop execution at that point so you can examine data at that location.
If you have ever used a debugger you are probably familiar with the concept of breakpoints. Basically, a breakpoint is a line in the source code where the debugger should break execution. In our example, we want to look at the code in `main()` so we would want to set a breakpoint at line 7 of malloc.cpp. Since you may not know the exact line number, you can also tell the debugger which function to break in using `break` or `b` command. Here is what we want to type for our example:

    (gdb) b segfault.c:7
    Breakpoint 1 at 0x40064d: file segfault.c, line 7.
    (gdb) 

So now Breakpoint 1 is set at main.cpp, line 9 as desired. (The reason the breakpoint gets a number is so we can refer to the breakpoint later, for example if we want to delete it.) So when the program is run, it will return control to the debugger everytime it reaches line 9.

This may not be desirable if the method is called many times but only has problems with certain values that are passed. Conditional breakpoints can help us here. For our example, we know that the program crashes when `fgets` is called with a value of `NULL`. So we might want to tell the debugger to only break at line 9 if `buf` is equal to `NULL`. This can be done by issuing the following command:

    (gdb) condition 1 buf==0
    (gdb) 

This basically says *Only break at Breakpoint 1 if the value of `item_to_remove` is 1.* Now we can run the program and know that the debugger will only break here when the specified condition is true.

##  Stepping

Continuing with the example above, we have set a conditional breakpoint and now want to go through this method one line at a time and see if we can locate the source of the error. This is accomplished using the `step` or `s` command to step in and using the `next` or `n` command to step over. gdb has the nice feature that when enter is pressed without typing a command, the last command is automatically used. That way we can step through by simply tapping the enter key after the first step has been entered. Here is what this looks like:

    (gdb) r
    The program being debugged has been started already.
    Start it from the beginning? (y or n) y
    Starting program: /home/hcli/Projects/Rholais.github.io/csci3150/gdb/src/segfault 

    Breakpoint 1, main (argc=1, argv=0x7fffffffde48) at segfault.c:8
    8	    buf = malloc(1<<31);
    (gdb) n
    10	    fgets(buf, 1024, stdin);
    (gdb) p buf
    $2 = 0x0
    (gdb) 

After typing `run`, `gdb` asks us if we want to restart the program, which we do. It then proceeds to run and breaks at the desired location in the program. Then we type `n` and proceed to hit enter to step through the program. At line 8 `buf` is set to `NULL`. Since the program can't access memory location `NULL`, the seg fault occurs. It is left as an exercise to the reader to fix this bug. (I've always wanted to say that. ;)

`gdb` can be exited by typing `quit` or `q`.

    (gdb) q
    A debugging session is active.

	    Inferior 1 [process 5912] will be killed.

    Quit anyway? (y or n) y


