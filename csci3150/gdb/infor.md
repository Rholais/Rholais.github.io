# Further Information

##  Supported Languages
gdb supports C, C++, D, Go, Objective-C, Fortran, OpenCL C, Pascal, Rust, assembly, Modula-2, and Ada. Some gdb features may be used in expressions regardless of the language you use: the gdb `@` and `::` operators, and the `{type}addr` construct can be used with the constructs of any supported language.

##  Compiling for Debugging
In order to debug a program effectively, you need to generate debugging information when you compile it. This debugging information is stored in the object file; it describes the data type of each variable or function and the correspondence between source line numbers and addresses in the executable code.

To tell GCC to emit extra information for use by a debugger, in almost all cases you need only to add `-g` to your other options. Alternatively, add `-ggdb` to produce debugging information for use by GDB.
