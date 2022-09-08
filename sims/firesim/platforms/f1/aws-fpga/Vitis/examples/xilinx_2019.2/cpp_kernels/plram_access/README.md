PLRAM Memory Access (C)
======================

This example shows the usage of PLRAM and how to use it with simple matrix multiplication (Row x Col).

***KEY CONCEPTS:*** Vitis Memory Hierarchy, PLRAMs

***KEYWORDS:*** PLRAM

## EXCLUDED PLATFORMS
Platforms containing following strings in their names are not supported for this example :
```
zc
xilinx_u50_xdma
samsung
```

##  DESIGN FILES
Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

```
src/host.cpp
src/mmult.cpp
```

##  COMMAND LINE ARGUMENTS
Once the environment has been configured, the application can be executed by
```
./host <mmult XCLBIN>
```

