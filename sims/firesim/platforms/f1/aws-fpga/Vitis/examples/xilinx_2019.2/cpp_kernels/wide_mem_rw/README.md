Wide Memory Read/Write (C)
======================

This is simple example of vector addition to demonstrate Wide Memory Access using ap_uint<512> data type. Based on input argument type, V++ compiler will figure our the memory datawidth between Global Memory and Kernel. For this example, ap_uint<512> datatype is used, so Memory datawidth will be 16 x (integer bit size) = 16 x 32 = 512 bit.

***KEY CONCEPTS:*** Kernel to DDR, wide memory access, burst read and write

***KEYWORDS:*** ap_uint<>, ap_int.h

##  DESIGN FILES
Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

```
src/host.cpp
src/vadd.cpp
```

##  COMMAND LINE ARGUMENTS
Once the environment has been configured, the application can be executed by
```
./host <vadd XCLBIN>
```

