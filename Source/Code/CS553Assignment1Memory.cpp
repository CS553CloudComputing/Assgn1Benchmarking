//============================================================================
// Name        : CS553.cpp
// Author      : Karan Jeet Singh
// Version     :
// Copyright   : Your copyright notice
// Description : Mem benchmark in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define TIMES 11
#define ITRN  100000000;
#define MAXTHREADS 16

using namespace std;

struct arg
{
	long blockSize;
	int threadNo;
	int loopCnt;
	long itrn;
};

class MemSeq
{
 	void MemorySequentialAccess_RW_Benchmark(long,long);
	void MemoryReadAccess_RW_Benchmark(long,long);
	long GetTimeSpent(struct timeval, struct timeval);
	long GenerateRandNum(long);

	public:
	long MemorySequentialAccess_RW(long,long);
	long MemoryRandomAccess_RW(long,long);
}memseq;


long MemSeq::GenerateRandNum(long upperLimit)
{
  srand(time(NULL));
  long random = rand()%upperLimit;
  return random;
}


long MemSeq::GetTimeSpent(struct timeval end,struct timeval start)
{
	return (end.tv_sec - start.tv_sec)*1000000 + ((int)end.tv_usec - (int)start.tv_usec);
}

void MemSeq::MemoryReadAccess_RW_Benchmark(long blockSize,long itrn)
{
	char **memRead = new char*[itrn];
	char **memWrite = new char*[itrn];
	long randNum;
	for(long i=0;i<itrn;i++)
	{
		randNum = GenerateRandNum(itrn);
		memRead[randNum]=new char[blockSize];
		memWrite[randNum]=new char[blockSize];
		memcpy(memWrite[randNum],memRead[randNum],blockSize);
	}
}

long MemSeq::MemoryRandomAccess_RW(long blockSize,long itrn)
{
	struct timeval start,end;
	gettimeofday(&start,0x0);
	MemoryReadAccess_RW_Benchmark(blockSize,itrn);
	gettimeofday(&end,0x0);
	long timeSpent = GetTimeSpent(end,start);

	return timeSpent;
}

void MemSeq::MemorySequentialAccess_RW_Benchmark(long blockSize,long itrn)
{
	char **memRead = new char*[itrn];
	char **memWrite = new char*[itrn];
	for(long i=0;i<itrn;i++)
	{
		memRead[i]=new char[blockSize];
		memWrite[i]=new char[blockSize];
		memcpy(memWrite[i],memRead[i],blockSize);
	}
}

long MemSeq::MemorySequentialAccess_RW(long blockSize, long itrn)
{
	struct timeval start,end;
	gettimeofday(&start,0x0);
	MemorySequentialAccess_RW_Benchmark(blockSize,itrn);
	gettimeofday(&end,0x0);
	long timeSpent = GetTimeSpent(end,start);

	return timeSpent;
}

double LatencyInms(long result, long itrn)
{
  return double(((double)result/1000)/(double)itrn);
}

double ThroughputInMBps(long result,long blockSize,long itrn)
{
	return ((double)(blockSize*itrn)/(1000000))/(((double)result/1000000));
}

void * SequenceAccessThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	long result = memseq.MemorySequentialAccess_RW(arguments->blockSize,arguments->itrn);
	cout<<LatencyInms(result,arguments->itrn)<<"\tms \t\t"<<ThroughputInMBps(result,arguments->blockSize,arguments->itrn)<<"\tMBps\n";

	pthread_exit(NULL);
}

void * RandomAccessThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	long result = memseq.MemoryRandomAccess_RW(arguments->blockSize,arguments->itrn);
	cout<<LatencyInms(result,arguments->itrn)<<"\tms \t\t"<<ThroughputInMBps(result,arguments->blockSize,arguments->itrn)<<"\tMBps\n";

	pthread_exit(NULL);
}

int main(int argi, char *argc[]) {

	long blockSize;
	struct arg arguments;
	pthread_t threads[MAXTHREADS];

	std::string oType = argc[1];
	int opType = atoi(oType.c_str());
	std::string threadCnt = argc[2];
	int tCnt = atoi(threadCnt.c_str());
	std:string bsize = argc[3];
	blockSize = atol(bsize.c_str());
	long itrn = 20000000/blockSize;
	itrn/=tCnt;
	arguments.blockSize=blockSize;
	arguments.itrn=itrn;

	switch(opType)
	{
		case 1:
			for(int i=0;i<tCnt;i++)
			{
				arguments.blockSize = blockSize;
				arguments.threadNo=i;
				pthread_create(&threads[i],NULL,&SequenceAccessThread,(void *)&arguments);
			}
				break;
		case 2:
			for(int i=0;i<tCnt;i++)
			{
				arguments.blockSize = blockSize;
				arguments.threadNo=i;
				pthread_create(&threads[i],NULL,&RandomAccessThread,(void *)&arguments);
			}
			break;
		default: cout<<"Incorrect operation type specified, please use either 1 or 2 as second argument";
	}


	pthread_exit(NULL);
}
