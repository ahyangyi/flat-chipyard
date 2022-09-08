Loop Pipelining (C)
======================

This example demonstrates how loop pipelining can be used to improve the performance of a kernel.

***KEY CONCEPTS:*** Kernel Optimization, Loop Pipelining

***KEYWORDS:*** pragma HLS PIPELINE

##  DESIGN FILES
Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

```
src/host.cpp
src/vector_addition.cpp
src/vector_addition_BAD.cpp
```

##  COMMAND LINE ARGUMENTS
Once the environment has been configured, the application can be executed by
```
./loop_pipeline <vector_addition XCLBIN>
```

