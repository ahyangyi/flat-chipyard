Dataflow Function OpenCL (OpenCL Kernel)
========================================

This is simple example of vector addition to demonstrate Dataflow functionality in OpenCL Kernel. OpenCL Dataflow allows user to run multiple functions together to achieve higher throughput.

**KEY CONCEPTS:** `Function/Task Level Parallelism <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/optimizingperformance.html#cvc1523913889499>`__

**KEYWORDS:** `xcl_dataflow <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/opencl_programming.html#dmj1504034358681>`__, `xclDataflowFifoDepth <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitiscommandcompiler.html#fgh1568640841739__section_mh4_qf4_bjb>`__

EXCLUDED PLATFORMS
------------------

Platforms containing following strings in their names are not supported for this example :

::

   nodma

DESIGN FILES
------------

Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

::

   src/adder.cl
   src/host.cpp
   
COMMAND LINE ARGUMENTS
----------------------

Once the environment has been configured, the application can be executed by

::

   ./cl_dataflow_func <adder XCLBIN>

DETAILS
-------

This example demonstrates the use of ``xcl_dataflow`` attribute used to
implement task level parallelism in OpenCL kernels i.e.  multiple
functions can be pipelied to increase the throughput of the design.

Kernel ``adder`` uses 3 functions ``read_input`` to read inputs from
global memory, ``compute_add`` for addition of these inputs and
``write_output`` to write the results back to the global memory.

.. code:: cpp

   __attribute__ ((xcl_dataflow))
   void adder(__global int *in, __global int *out, int inc, int size)
   {
       int buffer_in[BUFFER_SIZE];
       int buffer_out[BUFFER_SIZE];

       read_input(in,buffer_in,size);
       compute_add(buffer_in,buffer_out,inc,size);
       write_result(out,buffer_out,size);
   }

For more comprehensive documentation, `click here <http://xilinx.github.io/Vitis_Accel_Examples>`__.