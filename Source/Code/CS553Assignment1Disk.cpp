//============================================================================
// Name        : CS553.cpp
// Author      : Karan Jeet Singh & Anushka Dhar
// Version     :
// Copyright   : Your copyright notice
// Description : Disk benchmark in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <locale>

#define TIMES 11
#define ITRN  10000000;
#define MAXTHREADS 16

using namespace std;

struct arg
{
	long blockSize;
	int threadNo;
	int loopCnt;
	long itrn;
  char *blockSizeName;
  char *threadName;
};

class MemSeq
{
	void DiskSequentialAccess_Read_Benchmark(long,long,int);
	void DiskRandomAccess_Read_Benchmark(long,long,int);
	void DiskSequentialAccess_Write_Benchmark(long,long,int);
	void DiskRandomAccess_Write_Benchmark(long,long,int);
	long GetTimeSpent(struct timeval, struct timeval);

	public:
	long DiskSequentialAccess_Read(long,long,int);
	long DiskRandomAccess_Read(long,long,int);
	long DiskSequentialAccess_Write(long,long,int);
	long DiskRandomAccess_Write(long,long,int);
}memseq;


long MemSeq::GetTimeSpent(struct timeval end,struct timeval start)
{
	return (end.tv_sec - start.tv_sec)*1000000 + ((int)end.tv_usec - (int)start.tv_usec);
}

void MemSeq::DiskRandomAccess_Read_Benchmark(long blockSize,long itrn, int threadCnt)
{
	char *fileContent =new char[blockSize];
	char fileName[100];
	snprintf(fileName,100,"DiskRan%d.txt",(blockSize));
	ifstream inFile(fileName);
	inFile.seekg(0,inFile.end);
	unsigned int size = inFile.tellg();
	unsigned int pos;
	if(inFile.is_open())
	for(long i=0;i<itrn;i++)
	{
		srand(time(NULL));
		pos=(size!=0)?rand()%size:0;
		inFile.seekg(pos);
		inFile.read(fileContent,blockSize);
	}
	else cout<<"Cannot open file";
	inFile.close();
}

long MemSeq::DiskRandomAccess_Read(long blockSize,long itrn, int threadCnt)
{
	struct timeval start,end;
	gettimeofday(&start,0x0);
	DiskRandomAccess_Read_Benchmark(blockSize,itrn,threadCnt);
	gettimeofday(&end,0x0);
	long timeSpent = GetTimeSpent(end,start);

	return timeSpent;
}

void MemSeq::DiskSequentialAccess_Read_Benchmark(long blockSize,long itrn, int threadCnt)
{
	char *fileContent =new char[blockSize];
	char fileName[100];
	snprintf(fileName,100,"DiskS%d.txt",(blockSize));
	ifstream inFile(fileName);
	inFile.seekg(0,inFile.beg);
	if(inFile.is_open())
	for(long i=0;i<itrn;i++)
	{
		inFile.read(fileContent,blockSize);
	}
	else cout<<"Cannot open file";
	inFile.close();
}

long MemSeq::DiskSequentialAccess_Read(long blockSize, long itrn, int threadCnt)
{
	struct timeval start,end;
	gettimeofday(&start,0x0);
	DiskSequentialAccess_Read_Benchmark(blockSize,itrn,threadCnt);
	gettimeofday(&end,0x0);
	long timeSpent = GetTimeSpent(end,start);

	return timeSpent;
}

void MemSeq::DiskRandomAccess_Write_Benchmark(long blockSize,long itrn, int threadCnt)
{
	char *fileContent =new char[blockSize];
	char fileName[100];
	snprintf(fileName,100,"DiskRan%d.txt",(blockSize));
	for(long i=0;i<blockSize;i++)
		fileContent[i]=i%100;
	ofstream outFile(fileName,ios::out|ios::trunc);
	srand(time(NULL));
	unsigned int pos,size;
	if(outFile.is_open())
	for(long i=0;i<itrn;i++)
	{
		outFile.seekp(0,outFile.end);
		size = outFile.tellp();
		pos=(size>0)?rand()%size:0;
		outFile.seekp(pos);
		outFile.write(fileContent,blockSize);
	}
	outFile.close();
}

long MemSeq::DiskRandomAccess_Write(long blockSize,long itrn, int threadCnt)
{
	struct timeval start,end;
	gettimeofday(&start,0x0);
	DiskRandomAccess_Write_Benchmark(blockSize,itrn,threadCnt);
	gettimeofday(&end,0x0);
	long timeSpent = GetTimeSpent(end,start);

	return timeSpent;
}

void MemSeq::DiskSequentialAccess_Write_Benchmark(long blockSize,long itrn, int threadCnt)
{
	char *fileContent =new char[blockSize];
	streampos size;
	char fileName[100]="";
	snprintf(fileName,100,"DiskS%d.txt",(blockSize));
	for(long i=0;i<blockSize;i++)
		fileContent[i]=i%100;
	ofstream outFile(fileName,ios::out|ios::trunc);
	if(outFile.is_open())
	for(long i=0;i<itrn;i++)
	{
		outFile.write(fileContent,blockSize);
	}
	outFile.close();
}

long MemSeq::DiskSequentialAccess_Write(long blockSize, long itrn, int threadCnt)
{
	struct timeval start,end;
	gettimeofday(&start,0x0);
	DiskSequentialAccess_Write_Benchmark(blockSize,itrn,threadCnt);
	gettimeofday(&end,0x0);
	long timeSpent = GetTimeSpent(end,start);

	return timeSpent;
}


double LatencyInms(long result, long itrn)
{
  return double((double)result/1000)/(double)itrn;
}

double ThroughputInMBps(long result,long blockSize,long itrn)
{
	return ((double)(blockSize*itrn)/(1024*1024))/(((double)result/1000000));
}

void * SequenceAccessReadThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	long result = memseq.DiskSequentialAccess_Read(arguments->blockSize,arguments->itrn,arguments->threadNo);
	cout<<LatencyInms(result,arguments->itrn)<<"\tms \t\t"<<ThroughputInMBps(result,arguments->blockSize,arguments->itrn)<<"\tMBps\n";

	pthread_exit(NULL);
}

void * RandomAccessReadThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	long result = memseq.DiskRandomAccess_Read(arguments->blockSize,arguments->itrn,arguments->threadNo);
	cout<<LatencyInms(result,arguments->itrn)<<"\tms \t\t"<<ThroughputInMBps(result,arguments->blockSize,arguments->itrn)<<"\tMBps\n";

	pthread_exit(NULL);
}

void * SequenceAccessWriteThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	long result = memseq.DiskSequentialAccess_Write(arguments->blockSize,arguments->itrn,arguments->threadNo);
	cout<<LatencyInms(result,arguments->itrn)<<"\tms \t\t"<<ThroughputInMBps(result,arguments->blockSize,arguments->itrn)<<"\tMBps\n";

	pthread_exit(NULL);
}

void * RandomAccessWriteThread(void * args)
{
	struct arg *arguments;
	arguments = (struct arg *)args;
	long result = memseq.DiskRandomAccess_Write(arguments->blockSize,arguments->itrn,arguments->threadNo);
	cout<<LatencyInms(result,arguments->itrn)<<"\tms \t\t"<<ThroughputInMBps(result,arguments->blockSize,arguments->itrn)<<"\tMBps\n";

	pthread_exit(NULL);
}

int main(int argi, char *argc[]) {

	long blockSize;
	struct arg arguments;
	pthread_t threads[MAXTHREADS];

	std::string rw = argc[1];
	int rwOp = atoi(rw.c_str());
	std::string oType = argc[2];
	int opType = atoi(oType.c_str());
	std::string threadCnt = argc[3];
	int tCnt = atoi(threadCnt.c_str());
	std::string bsize = argc[4];
	blockSize = atol(bsize.c_str());
	long itrn = 10000000/blockSize;
	itrn/=tCnt;
	arguments.blockSize=blockSize;
	arguments.itrn=itrn;
	arguments.threadName = argc[3];
	arguments.blockSizeName = argc[4];
	
	if(rwOp==2&&opType==1)
		for(int i=1;i<=tCnt;i++)
			{
				arguments.threadNo=i;
				pthread_create(&threads[i],NULL,&SequenceAccessReadThread,(void *)&arguments);
			}
	else if(rwOp==2&&opType==2)
		for(int i=1;i<=tCnt;i++)
			{
				arguments.threadNo=i;
				pthread_create(&threads[i],NULL,&RandomAccessReadThread,(void *)&arguments);
			}

	else if(rwOp==1&&opType==1)
		for(int i=1;i<=tCnt;i++)
			{
				arguments.threadNo=i;
				pthread_create(&threads[i],NULL,&SequenceAccessWriteThread,(void *)&arguments);
			}
	else if(rwOp==1&&opType==2)
		for(int i=1;i<=tCnt;i++)
			{
				arguments.threadNo=i;
				pthread_create(&threads[i],NULL,&RandomAccessWriteThread,(void *)&arguments);
			}
	pthread_exit(NULL);
}
