Bandwidth Test (C)
===================

This example contains bandwidth test kernels to validate FPGA DDR.

Extracting Memory Information and generate cfg file:
   
      - Platforminfo -j (path to xpfm) > platform_info.json
      - From the platform_info.json file we pick the memory info
      - Generate the meta data related to Memory Banks(DDR/HBM/HOST) to platform_bandwidth.cfg file


Using the ``sp`` option  in the platform_bandwidth.cfg file AXI-Master Port is connected to the IP:

::

   sp=bandwidth_1.input:DDR[0]
   sp=bandwidth_1.output:DDR[0]
   sp=bandwidth_2.input:DDR[1]
   sp=bandwidth_2.output:DDR[1]
   sp=bandwidth_3.input:DDR[2]
   sp=bandwidth_3.output:DDR[2]
   sp=bandwidth_4.input:DDR[3]
   sp=bandwidth_4.output:DDR[3]
