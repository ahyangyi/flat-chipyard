Multiple Compute Units (C) 
===========================

This is simple Example of Multiple Compute units to showcase how a single kernel can be instantiated into Multiple compute units. Host code will show how to use multiple compute units and run them concurrently.

**KEY CONCEPTS:** `Multiple Compute Units <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/opencl_programming.html#dqz1555367565037>`__

**KEYWORDS:** `nk <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html#clt1568640709907__section_wtp_zvm_1jb>`__

EXCLUDED PLATFORMS
------------------

Platforms containing following strings in their names are not supported for this example :

::

   nodma

DESIGN FILES
------------

Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

::

   src/host.cpp
   src/vadd.cpp
   
COMMAND LINE ARGUMENTS
----------------------

Once the environment has been configured, the application can be executed by

::

   ./mult_compute_units <vadd XCLBIN>

DETAILS
-------

This example explains how to create multiple instances of a kernel and
execute them concurrently.

For the same kernel to be instantiated into multiple compute units,
``nk`` flag is used to specify the number of compute units in a ``.cfg``
config file as shown below:

::

   [connectivity]
   nk=vadd:4

For kernels to execute concurrently, command queue is initialized with
out of order execution mode enabled.

\```c++ OCL_CHECK(err, q = cl::CommandQueue(context, device,
CL_QUEUE_PROFILING_ENABLE \| CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
&err));

For more comprehensive documentation, `click here <http://xilinx.github.io/Vitis_Accel_Examples>`__.