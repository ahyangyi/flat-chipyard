Critical Path (C)
======================

This example shows a normal coding style which could lead to critical path issue and design will give degraded timing.  Example also contains better coding style which can improve design timing.

***KEY CONCEPTS:*** Critical Path handling, Improve Timing

##  DESIGN FILES
Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

```
data/golden.bmp
data/input.bmp
src/apply_watermark.cpp
src/host.cpp
```

##  COMMAND LINE ARGUMENTS
Once the environment has been configured, the application can be executed by
```
./host <apply_watermark_GOOD XCLBIN> ./data/input.bmp ./data/golden.bmp
```

