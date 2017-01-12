# Debugging

##  When to Use a Debugger

Debugging is something that can't be avoided. Every programmer will at one point in their programming career have to debug a section of code. There are many ways to go about debugging, from printing out messages to the screen, using a debugger, or just thinking about what the program is doing and making an educated guess as to what the problem is.

Before a bug can be fixed, the source of the bug must be located. For example, with segmentation faults, it is useful to know on which line of code the seg fault is occuring. Once the line of code in question has been found, it is useful to know about the values in that method, who called the method, and why (specifically) the error is occuring. Using a debugger makes finding all of this information very simple.

Go ahead and make the program for this tutorial, and run the program. The program will print out some messages, and then it will print that it has received a segmentation fault signal, resulting in a program crash. Given the information on the screen at this point, it is near impossible to determine why the program crashed, much less how to fix the problem. We will now begin to debug this program.

##  Loading a Program

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
    The fully created list is:
    4
    3
    2
    1

    Now removing elements:
    4
    3
    2
    1


    Program received signal SIGSEGV, Segmentation fault.
    0x00000000004007f5 in remove_item (l=0x602010, item_to_remove=1) at main.c:75
    75			marker = marker->next_;
    (gdb) 

The program crashed so lets see what kind of information we can gather.

## Registers

You can refer to machine register contents, in expressions, as variables with names starting with `$`. The names of registers are different for each machine; use `info registers` or `i r` to see the names used on your machine.

    (gdb) i r
    rax            0x0	0
    rbx            0x0	0
    rcx            0x7ffff7b046e0	140737348912864
    rdx            0xa	10
    rsi            0x1	1
    rdi            0x602010	6299664
    rbp            0x7fffffffddf0	0x7fffffffddf0
    rsp            0x7fffffffddc0	0x7fffffffddc0
    r8             0x7ffff7dd3780	140737351858048
    r9             0x7ffff7fd1700	140737353946880
    r10            0x1d6	470
    r11            0x246	582
    r12            0x400550	4195664
    r13            0x7fffffffdf00	140737488346880
    r14            0x0	0
    r15            0x0	0
    rip            0x4007f5	0x4007f5 <remove_item+331>
    eflags         0x10206	[ PF IF RF ]
    cs             0x33	51
    ss             0x2b	43
    ds             0x0	0
    es             0x0	0
    fs             0x0	0
    gs             0x0	0
    (gdb)

Names and values of all registers except floating-point and vector registers (in the current stack frame) are printed.

##  Inspecting Crashes

So already we can see the that the program was at line 75 of main.cpp and we can see the line of code that was executed. But we also want to know who called this method and we would like to be able to examine values in the calling methods. So at the `gdb` prompt, we type `backtrace` or `bt` which gives me the following output:

    #0  0x00000000004007f5 in remove_item (l=0x602010, item_to_remove=1) at main.c:75
    #1  0x000000000040096b in main (argc=1, argv=0x7fffffffdf08) at main.c:117

We can use the `frame` or `f` command using the stack frame number or the address of the frame as a parameter to select and print a stack frame and use the `print EXP` or `p EXP` command Print value of expression `EXP`. Here is what happens when I run the command:

    (gdb) f 0
    #0  0x00000000004007f5 in remove_item (l=0x602010, item_to_remove=1) at main.c:75
    75			marker = marker->next_;
    (gdb) p marker
    $1 = (struct Node *) 0x0
    (gdb) 

So the program is crashing while trying to run `remove_item` with a parameter of `1`. We have now narrowed the problem down to a specific function and a specific value for the parameter.

##  Conditional Breakpoints

Now that we know where and when the segfault is occuring, we want to watch what the program is doing right before it crashes. One way to do this is to step through, one at a time, every statement of the program until we get to the point of execution where we want to see what is happening. This works, but sometimes you may want to just run to a particular section of code and stop execution at that point so you can examine data at that location.
If you have ever used a debugger you are probably familiar with the concept of breakpoints. Basically, a breakpoint is a line in the source code where the debugger should break execution. In our example, we want to look at the code in `LinkedList<int>::remove()` so we would want to set a breakpoint at line 58 of main.cpp. Since you may not know the exact line number, you can also tell the debugger which function to break in using `break` or `b` command. Here is what we want to type for our example:

    (gdb) b remove_item
    Breakpoint 1 at 0x4006b9: file main.c, line 41.
    (gdb) 

So now Breakpoint 1 is set at main.cpp, line 41 as desired. (The reason the breakpoint gets a number is so we can refer to the breakpoint later, for example if we want to delete it.) So when the program is run, it will return control to the debugger everytime it reaches line 41.

This may not be desirable if the method is called many times but only has problems with certain values that are passed. Conditional breakpoints can help us here. For our example, we know that the program crashes when `remove_item` is called with a value of `1`. So we might want to tell the debugger to only break at line 41 if `item_to_remove` is equal to `1`. This can be done by issuing the following command:

    (gdb) condition 1 item_to_remove==1
    (gdb) 

This basically says *Only break at Breakpoint 1 if the value of `item_to_remove` is 1.* Now we can run the program and know that the debugger will only break here when the specified condition is true.

##  Stepping

Continuing with the example above, we have set a conditional breakpoint and now want to go through this method one line at a time and see if we can locate the source of the error. This is accomplished using the step command. gdb has the nice feature that when enter is pressed without typing a command, the last command is automatically used. That way we can step through by simply tapping the enter key after the first step has been entered. Here is what this looks like:

    (gdb) r
    The program being debugged has been started already.
    Start it from the beginning? (y or n) y
    Starting program: /home/hcli/Projects/Rholais.github.io/csci3150/gdb/src/main 
    The fully created list is:
    4
    3
    2
    1

    Now removing elements:
    4
    3
    2
    1


    Breakpoint 1, remove_item (l=0x602010, item_to_remove=1) at main.c:41
    41		struct Node *marker = l->head_;
    (gdb) step
    42		struct Node *temp = NULL;  // temp points to one behind as we iterate
    (gdb) 
    44		while(marker) {
    (gdb) 
    45			if(marker->value_ == item_to_remove) {
    (gdb) 
    73			marker = NULL;  // reset the marker
    (gdb) 
    74			temp = marker;
    (gdb) 
    75			marker = marker->next_;
    (gdb) 

    Program received signal SIGSEGV, Segmentation fault.
    0x00000000004007f5 in remove_item (l=0x602010, item_to_remove=1) at main.c:75
    75			marker = marker->next_;
    (gdb) 

After typing `run`, `gdb` asks us if we want to restart the program, which we do. It then proceeds to run and breaks at the desired location in the program. Then we type step and proceed to hit enter to step through the program. Note that the debugger steps into functions that are called. If you don't want to do this, you can use next instead of step which otherwise has the same behavior.
The error in the program is obvious. At line 73 `marker` is set to `NULL`, but at line 75 a member of `marker` is accessed. Since the program can't access memory location `NULL`, the seg fault occurs. In this example, nothing has to be done to `marker` and the error can be avoided by simply removing line 73 from main.c.

If you look at the output from running the program, you will see first of all that the program runs without crashing, but there is a memory leak somewhere in the program. (Hint: It is in the `remove_item` function. One of the cases for remove doesn't work properly.) It is left as an exercise to the reader to use the debugger in locating and fixing this bug. (I've always wanted to say that. ;)

`gdb` can be exited by typing `quit` or `q`.

    (gdb) q
    A debugging session is active.

	    Inferior 1 [process 6703] will be killed.

    Quit anyway? (y or n) y


