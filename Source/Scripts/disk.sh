#!/bin/bash
echo "... Running Disk Benchmark ... "
#g++ CS553Assignment1Disk.cpp -o disk -pthread -fpermissive -std=c++11
echo ""
echo "Write Sequential 1B  1Thread"
./disk 1 1 1 1
echo "Write Sequential 10B 1Thread"
./disk 1 1 1 10
echo "Write Sequential 100B 1Thread"
./disk 1 1 1 100
echo "Write Sequential 1KB  1Thread"
./disk 1 1 1 1000
echo "Write Sequential 10KB 1Thread"
./disk 1 1 1 10000
echo "Write Sequential 100KB 1Thread"
./disk 1 1 1 100000
echo "Write Sequential 1MB 1Thread"
./disk 1 1 1 1000000
echo "Write Random     1B  1Thread"
./disk 1 2 1 1
echo "Write Random     10B 1Thread"
./disk 1 2 1 10
echo "Write Random     100B 1Thread"
./disk 1 2 1 100
echo "Write Random     1KB 1Thread"
./disk 1 2 1 1000
echo "Write Random     10KB 1Thread"
./disk 1 2 1 10000
echo "Write Random     100KB 1Thread"
./disk 1 2 1 100000
echo "Write Random     1MB 1Thread"
./disk 1 2 1 1000000
echo "Read  Sequential 1B  1Thread"
./disk 2 1 1 1
echo "Read  Sequential 10B 1Thread"
./disk 2 1 1 10
echo "Read  Sequential 100B 1Thread"
./disk 2 1 1 100
echo "Read  Sequential 1KB 1Thread"
./disk 2 1 1 1000
echo "Read  Sequential 10KB 1Thread"
./disk 2 1 1 10000
echo "Read  Sequential 100KB 1Thread"
./disk 2 1 1 100000
echo "Read  Sequential 1MB 1Thread"
./disk 2 1 1 1000000
echo "Read  Random     1B  1Thread"
./disk 2 2 1 1
echo "Read  Random     10B 1Thread"
./disk 2 2 1 10
echo "Read  Random     100B 1Thread"
./disk 2 2 1 100
echo "Read  Random     1KB 1Thread"
./disk 2 2 1 1000
echo "Read  Random     10KB 1Thread"
./disk 2 2 1 10000
echo "Read  Random     100KB 1Thread"
./disk 2 2 1 100000
echo "Read  Random     1MB 1Thread"
./disk 2 2 1 1000000
echo "Write Sequential 1B  2Thread"
./disk 1 1 2 1
echo "Write Sequential 10B 2Thread"
./disk 1 1 2 10
echo "Write Sequential 100B 2Thread"
./disk 1 1 2 100
echo "Write Sequential 1KB  2Thread"
./disk 1 1 2 1000
echo "Write Sequential 10KB 2Thread"
./disk 1 1 2 10000
echo "Write Sequential 100KB 2Thread"
./disk 1 1 2 100000
echo "Write Sequential 1MB 2Thread"
./disk 1 1 2 1000000
echo "Write Random     1B  2Thread"
./disk 1 2 2 1
echo "Write Random     10B 2Thread"
./disk 1 2 2 10
echo "Write Random     100B 2Thread"
./disk 1 2 2 100
echo "Write Random     1KB 2Thread"
./disk 1 2 2 1000
echo "Write Random     10KB 2Thread"
./disk 1 2 2 10000
echo "Write Random     100KB 2Thread"
./disk 1 2 2 100000
echo "Write Random     1MB 2Thread"
./disk 1 2 2 1000000
echo "Read  Sequential 1B  2Thread"
./disk 2 1 2 1
echo "Read  Sequential 10B 2Thread"
./disk 2 1 2 10
echo "Read  Sequential 100B 2Thread"
./disk 2 1 2 100
echo "Read  Sequential 1KB 2Thread"
./disk 2 1 2 1000
echo "Read  Sequential 10KB 2Thread"
./disk 2 1 2 10000
echo "Read  Sequential 100KB 2Thread"
./disk 2 1 2 100000
echo "Read  Sequential 1MB 2Thread"
./disk 2 1 2 1000000
echo "Read  Random     1B  2Thread"
./disk 2 2 2 1
echo "Read  Random     10B 2Thread"
./disk 2 2 2 10
echo "Read  Random     100B 2Thread"
./disk 2 2 2 100
echo "Read  Random     1KB 2Thread"
./disk 2 2 2 1000
echo "Read  Random     10KB 2Thread"
./disk 2 2 2 10000
echo "Read  Random     100KB 2Thread"
./disk 2 2 2 100000
echo "Read  Random     1MB 2Thread"
./disk 2 2 2 1000000
echo "Write Sequential 1B  4Thread"
./disk 1 1 4 1
echo "Write Sequential 10B 4Thread"
./disk 1 1 4 10
echo "Write Sequential 100B 4Thread"
./disk 1 1 4 100
echo "Write Sequential 1KB  4Thread"
./disk 1 1 4 1000
echo "Write Sequential 10KB 4Thread"
./disk 1 1 4 10000
echo "Write Sequential 100KB 4Thread"
./disk 1 1 4 100000
echo "Write Sequential 1MB 4Thread"
./disk 1 1 4 1000000
echo "Write Random     1B  4Thread"
./disk 1 2 4 1
echo "Write Random     10B 4Thread"
./disk 1 2 4 10
echo "Write Random     100B 4Thread"
./disk 1 2 4 100
echo "Write Random     1KB 4Thread"
./disk 1 2 4 1000
echo "Write Random     10KB 4Thread"
./disk 1 2 4 10000
echo "Write Random     100KB 4Thread"
./disk 1 2 4 100000
echo "Write Random     1MB 4Thread"
./disk 1 2 4 1000000
echo "Read  Sequential 1B  4Thread"
./disk 2 1 4 1
echo "Read  Sequential 10B 4Thread"
./disk 2 1 4 10
echo "Read  Sequential 100B 4Thread"
./disk 2 1 4 100
echo "Read  Sequential 1KB 4Thread"
./disk 2 1 4 1000
echo "Read  Sequential 10KB 4Thread"
./disk 2 1 4 10000
echo "Read  Sequential 100KB 4Thread"
./disk 2 1 4 100000
echo "Read  Sequential 1MB 4Thread"
./disk 2 1 4 1000000
echo "Read  Random     1B  4Thread"
./disk 2 2 4 1
echo "Read  Random     10B 4Thread"
./disk 2 2 4 10
echo "Read  Random     100B 4Thread"
./disk 2 2 4 100
echo "Read  Random     1KB 4Thread"
./disk 2 2 4 1000
echo "Read  Random     10KB 4Thread"
./disk 2 2 4 10000
echo "Read  Random     100KB 4Thread"
./disk 2 2 4 100000
echo "Read  Random     1MB 4Thread"
./disk 2 2 4 1000000
echo "Write Sequential 1B  8Thread"
./disk 1 1 8 1
echo "Write Sequential 10B 8Thread"
./disk 1 1 8 10
echo "Write Sequential 100B 8Thread"
./disk 1 1 8 100
echo "Write Sequential 1KB  8Thread"
./disk 1 1 8 1000
echo "Write Sequential 10KB 8Thread"
./disk 1 1 8 10000
echo "Write Sequential 100KB 8Thread"
./disk 1 1 8 100000
echo "Write Sequential 1MB 8Thread"
./disk 1 1 8 1000000
echo "Write Random     1B  8Thread"
./disk 1 2 8 1
echo "Write Random     10B 8Thread"
./disk 1 2 8 10
echo "Write Random     100B 8Thread"
./disk 1 2 8 100
echo "Write Random     1KB 8Thread"
./disk 1 2 8 1000
echo "Write Random     10KB 8Thread"
./disk 1 2 8 10000
echo "Write Random     100KB 8Thread"
./disk 1 2 8 100000
echo "Write Random     1MB 8Thread"
./disk 1 2 8 1000000
echo "Read  Sequential 1B  8Thread"
./disk 2 1 8 1
echo "Read  Sequential 10B 8Thread"
./disk 2 1 8 10
echo "Read  Sequential 100B 8Thread"
./disk 2 1 8 100
echo "Read  Sequential 1KB 8Thread"
./disk 2 1 8 1000
echo "Read  Sequential 10KB 8Thread"
./disk 2 1 8 10000
echo "Read  Sequential 100KB 8Thread"
./disk 2 1 8 100000
echo "Read  Sequential 1MB 8Thread"
./disk 2 1 8 1000000
echo "Read  Random     1B  8Thread"
./disk 2 2 8 1
echo "Read  Random     10B 8Thread"
./disk 2 2 8 10
echo "Read  Random     100B 8Thread"
./disk 2 2 8 100
echo "Read  Random     1KB 8Thread"
./disk 2 2 8 1000
echo "Read  Random     10KB 8Thread"
./disk 2 2 8 10000
echo "Read  Random     100KB 8Thread"
./disk 2 2 8 100000
echo "Read  Random     1MB 8Thread"
./disk 2 2 8 1000000
echo "... Benchmarking complete ... "