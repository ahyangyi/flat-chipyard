Kernel Swap
============

This example shows how host can swap the kernels and share the same buffer between two kernels which exist in separate binary containers. All the Vitis platforms do not persist the buffer data if device is reprogrammed by different binary. So host has to migrate data from device to host memory before swapping the next binary. After re-programming the binary, host has to migrate the buffer back to device for next kernel operation.

In design, host application first programs the device using binary containing `krnl_vmult` and executes the operation. 

Pointer `h_temp` in host is used to store the result of first kernel and provide the data to next kernel operation.

During first kernel call, `d_mul` buffer is created using `h_temp` buffer. 

```c++
cl::Buffer d_mul(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, sizeof(int) * LENGTH, h_temp.data(), &err);
```                   

Once kernel finishes the operation, result of the kernel will be read from device to host into `h_temp` buffer.

```c++
q.enqueueMigrateMemObjects({d_mul},CL_MIGRATE_MEM_OBJECT_HOST);
```   

After this, Host is ready to reprogram the 2nd Binary.
```c++
cl::Program program(context, devices, vadd_bins);
```
After reprogramming with new binary, a new buffer `d_temp` will be created using same `h_temp` host pointer. 
```c++
cl::Buffer d_temp(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,   sizeof(int) * LENGTH, h_temp.data());
``` 
And data will be transfered from host to device for 2nd kernel's execution.
```c++
q.enqueueMigrateMemObjects({d_temp}, 0/* 0 means from host*/);
```
