------------------------
CS553 - Cloud computing
------------------------
Programming Assignment 1
------------------------

-----------------------------------------------------------------
This part tries to explain how to run the benchmarking programs.
-----------------------------------------------------------------

1. Execute CPU benchmarking program:
In order to execute the CPU Benchmarking program, go to "Source/Scripts" folder.
run the shell script titled "BenchmarkCPU.sh" by running the following command:
./BenchmarkCPU.sh

2. Execute GPU benchmarking program:
In order to execute the GPU Benchmarking program, go to "Source/Code" folder.
Before running the file you need to compile the kernel.cu file using the following commands:
operation used "r" read , "w" ,size of block in bytes,"s" for single thread FLOP,IOP calculation
"m" for multithread FLOP,IOP calculation
$ nvcc kernel.cu -o GPU
$ ./GPU <operation> <size of block>

to get the read throughtput and latency for 1000KB block
$ ./GPU r 1000

to get the write throughtput and latency for 100KB block
$ ./GPU w 100

to FLOPS/IOPS for single thread
$ ./GPU s

to get FLOPS/IOPS for single thread
$ ./GPU m


3. Execute Memory benchmarking program:
In order to execute the Memory Benchmarking program, go to "Source/Scripts" folder.
run the shell script titled "memory.sh" by running the following command:
./memory.sh

4. Execute Disk benchmarking program:
In order to execute the Disk Benchmarking program, go to "Source/Scripts" folder.
run the shell script titled "disk.sh" by running the following command:
./disk.sh

5. Execute Network benchmarking program:
In order to execute the CPU Benchmarking program, go to "Source/Scripts" folder.
Run the executable file labelled "network"
the argument it takes are as follows:

NOTE: The UDP server will not close automaticaly as it runs in zombie mode, and it can be used for all the client calls.

./network <server(1)/client(2)> <port(2000-9999)> <TCP(1)/UDP(2)> <Block Size> <Thread Count> <Server address(only required for client)>

To run a TCP server for 2 thread and 1KB
./network 1 8989 1 1000 2

To run a TCP client for 2 threads and 1KB
./network 2 8989 1 1000 2 127.0.0.1

To run a UDP server for 2 threads and 64KB
./network 1 8989 2 64000 2

To run a UDP client for 2 threads and 64KB
./network 2 8989 2 64000 2 127.0.0.1

--------------------------------------------------
This part specifies the compilation requirements
--------------------------------------------------

 - The memory.cpp file require boost libraries, hence the external library should be specied while compiling it via g++ compiler
 - The multithreading is implemented using pthread header, hence "-pthread" flag should be specified when compiling.
 - The source files are prepared using c++11 compiler, hence it needs to be explicitly specified when compiling.
 - The network.c should be compiled via gcc compiler as it is prepared using C, and not C++