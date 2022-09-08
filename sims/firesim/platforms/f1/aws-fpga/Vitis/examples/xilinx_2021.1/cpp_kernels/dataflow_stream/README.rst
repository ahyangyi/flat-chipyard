Dataflow Using HLS Stream
=========================

This is simple example of vector addition to demonstrate Dataflow functionality of HLS. HLS Dataflow allows user to schedule multiple task together to achieve higher throughput.

**KEY CONCEPTS:** `Task Level Parallelism <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/optimizingperformance.html#cvc1523913889499>`__

**KEYWORDS:** `dataflow <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/vitis_hls_optimization_techniques.html#bmx1539734225930>`__, `hls::stream <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/hls_stream_library.html>`__

EXCLUDED PLATFORMS
------------------

Platforms containing following strings in their names are not supported for this example :

::

   nodma

DESIGN FILES
------------

Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

::

   src/adder.cpp
   src/host.cpp
   
COMMAND LINE ARGUMENTS
----------------------

Once the environment has been configured, the application can be executed by

::

   ./dataflow_stream <adder XCLBIN>

DETAILS
-------

This example explains how ``#pragma HLS dataflow`` can be used to
implement task level parallelism using ``HLS Stream`` datatype.

Usually data stored in the array is consumed or produced in a sequential
manner, a more efficient communication mechanism is to use streaming
data as specified by the ``STREAM`` pragma, where FIFOs are used instead
of RAMs. Depth of ``FIFO`` can be specified by ``depth`` option in the
pragma.

.. code:: cpp

   #pragma HLS STREAM variable = inStream depth = 32
   #pragma HLS STREAM variable = outStream depth = 32

Vector addition in kernel is divided into 3 sub-tasks(read, compute_add
and write) which are then performed concurrently using ``Dataflow``.

.. code:: cpp

   #pragma HLS dataflow
       read_input(in, inStream, size);
       compute_add(inStream, outStream, inc, size);
       write_result(out, outStream, size);

For more comprehensive documentation, `click here <http://xilinx.github.io/Vitis_Accel_Examples>`__.