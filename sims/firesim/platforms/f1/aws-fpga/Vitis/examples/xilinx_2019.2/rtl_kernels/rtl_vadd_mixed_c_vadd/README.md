Mixed Kernels (RTL & C)
======================

This example has one RTL kernel and one C kernel. Both RTL kernel and C kernel perform vector addition. The C kernel reads the output from RTL kernel as one of two inputs.

***KEY CONCEPTS:*** Mixed Kernels

##  DESIGN FILES
Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

```
src/hdl/krnl_vadd_rtl.v
src/hdl/krnl_vadd_rtl_adder.sv
src/hdl/krnl_vadd_rtl_axi_read_master.sv
src/hdl/krnl_vadd_rtl_axi_write_master.sv
src/hdl/krnl_vadd_rtl_control_s_axi.v
src/hdl/krnl_vadd_rtl_counter.sv
src/hdl/krnl_vadd_rtl_int.sv
src/host.cpp
src/kernel.xml
src/krnl_vadd.cpp
```

##  COMMAND LINE ARGUMENTS
Once the environment has been configured, the application can be executed by
```
./host <vadd XCLBIN>
```

