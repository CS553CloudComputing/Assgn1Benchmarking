#!/bin/bash
echo "... Running CPU Benchmark ... "
#g++ -std=c++11 '/home/kanwal/workspace/BenchmarkCPU/src/BenchmarkCPU.cpp' -o BenchmarkCPU -pthread

echo ""
echo "IOPS for 1 thread"
./BenchmarkCPU 1 i
echo "-------------------"
echo "IOPS for 2 thread"
./BenchmarkCPU 2 i
echo "-------------------"
echo "IOPS for 4 thread"
./BenchmarkCPU 4 i
echo "-------------------"
echo "IOPS for 8 thread"
./BenchmarkCPU 8 i
echo "-------------------"
echo "-------------------"
echo "FLOPS for 1 thread"
./BenchmarkCPU 1 f
echo "-------------------"
echo "FLOPS for 2 thread"
./BenchmarkCPU 2 f
echo "-------------------"
echo "FLOPS for 4 thread"
./BenchmarkCPU 4 f
echo "-------------------"
echo "FLOPS for 8 thread"
./BenchmarkCPU 8 f
echo "... Benchmarking complete ... "
echo "-------------------"
echo "-------------------"

