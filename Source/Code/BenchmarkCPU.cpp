//============================================================================
// Name        : BenchmarkCPU.cpp
// Author      : Kanwal Preet Singh
// Version     :
// Copyright   :
// Description : Benchmarking CPU performance IOPS, Ansi-style
//============================================================================

#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <string>

using namespace std;
#define MaxThreadCount 16
#define LoopCount 100000000
#define ResultSet 5
#define IOPS 'i'
#define FLOPS 'f'

void *RunOnThreadIOPs(void *);
void *RunOnThreadFLOPs(void *);

class BenchmarkCPU
{
	//Variable declartion
	int count = 0;
	long first = 1;//,next = 1,sum = 0;
	double firstD = 3.142;//857143//, nextD = 1.23, remD = 0.0,
	double countD = 0.1;

	//Method declartion
	void IOPs();
	void FLOPs();
	long GetTimeSpent(struct timeval, struct timeval);
public :
	long MeasureIOPs();
	long MeasureFLOPs();

}benchmarkCPUObj;

//method to calculate the time of execution
long BenchmarkCPU::GetTimeSpent(struct timeval end,struct timeval start)
{
    return (end.tv_sec - start.tv_sec)*1000000 + ((int)end.tv_usec - (int)start.tv_usec);
}

//method to perform no of IOP
void BenchmarkCPU::IOPs()
	{
		while(count<LoopCount)
		{
			first = count * 2 * 3 * 5 -7 + 9 + 11 -13 + 15 - 17;
			count++;
//			first 	*= first;
//			next	*= next;
//			sum = first + next;
//			sum *=sum;
//			next++;
//			first++;
//			first += next;
//			next += sum;
//			count++;
//			sum *= count;
		}
	}

//method to measure no of IOP
long BenchmarkCPU::MeasureIOPs()
{
	struct timeval start,end;
	long timespent;

	gettimeofday(&start,0x0);
	IOPs();
	gettimeofday(&end,0x0);
	timespent = GetTimeSpent(end,start);

	cout<<endl<<"Time in us : "<<timespent;
	cout<<endl<<"IOPS : "<<(long)(((double)(11*LoopCount)/(double)timespent)*1000000);
	cout<<endl;
	return (long)(((double)(10*LoopCount)/(double)timespent)*1000000);

}

//method to perform no of FLOP
void BenchmarkCPU::FLOPs()
	{
		while(countD<LoopCount)
		{
			firstD = 1.01 * 2.20 - 3.003 + 4.44 * 0.02 -0.0012 +5.001;
			/*remD = firstD / nextD;
			remD = firstD;
			nextD = firstD + remD;*/
			countD++;
		}
	}

//method to measure no of FLOP
long BenchmarkCPU::MeasureFLOPs()
{
	struct timeval start,end;
	long timespent;

	gettimeofday(&start,0x0);
	FLOPs();
	gettimeofday(&end,0x0);
	timespent = GetTimeSpent(end,start);

	cout<<endl<<"Time in us : "<<timespent;
	cout<<endl<<"FLOPS : "<<(long)(((double)(7*LoopCount)/(double)timespent)*1000000);
	cout<<endl;
	return (long)(((double)(7*LoopCount)/(double)timespent)*1000000);
}
class MultiThreading
{
public :
	void ParallelRunIOPs(int);
	void ParallelRunFLOPs(int);

}multiThreadObj;

void MultiThreading::ParallelRunIOPs(int numofThreads = 1 )
{
	int errorC;
	pthread_t threads[MaxThreadCount];

	for(int i =0 ; i<=numofThreads-1;i++)
	{
		errorC = pthread_create(&threads[i],NULL,&RunOnThreadIOPs,NULL);
		if(errorC)
		{
			cout<<endl<<"Error code : "<<errorC<<" for thread : "<<i<<endl;
			exit(0);
		}
	}
	pthread_exit(NULL);
}

void MultiThreading::ParallelRunFLOPs(int numofThreads = 1 )
{
	int errorC;
	pthread_t threadsFLOP[MaxThreadCount];

	for(int i =0 ; i<=numofThreads -1;i++)
	{
		errorC = pthread_create(&threadsFLOP[i],NULL,&RunOnThreadFLOPs,NULL);
		if(errorC)
		{
			cout<<endl<<"Error code : "<<errorC<<" for thread : "<<i<<endl;
			exit(0);
		}
	}
	pthread_exit(NULL);
}

void *RunOnThreadIOPs(void *)
{
	long resultsIOP;
	resultsIOP = benchmarkCPUObj.MeasureIOPs();
	pthread_exit(NULL);
}

void *RunOnThreadFLOPs(void *)
{
	long resultsFLOP;
	resultsFLOP	= benchmarkCPUObj.MeasureFLOPs();
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	int numofThreads;
	char val;
	numofThreads = atoi(argv[1]);
	val = *argv[2];

	if(val == IOPS )
	{
		multiThreadObj.ParallelRunIOPs(numofThreads);
	}

	if(val == FLOPS)
	{
		multiThreadObj.ParallelRunFLOPs(numofThreads);
	}
	pthread_exit(NULL);
	return 0;
}
