#include<stdio.h>

#define SIZE 10240
#define MEMSIZE 10000000
#define B 1024

struct arg
{
	long blockSize;
	int loopCnt;
	long itrn;
}arguments;

float MemoryBenchmarkWrite()
{
	char *a;
	char *d_a;

	cudaEvent_t startT, stopT;
	float millisecs = 0;

	cudaEventCreate(&startT);
	cudaEventCreate(&stopT);

	a = (char *)malloc(MEMSIZE*sizeof(char));
	cudaMalloc(&d_a, MEMSIZE*sizeof(char));

	cudaEventRecord(startT);

	for (int i = 0; i < arguments.itrn; i++)
	{
		cudaMemcpy(d_a + i * (arguments.blockSize), a + i * (arguments.blockSize), arguments.blockSize * sizeof(char), cudaMemcpyHostToDevice);
	}

	cudaEventRecord(stopT);

	cudaEventSynchronize(stopT);

	cudaEventElapsedTime(&millisecs, startT, stopT);

	printf("Time in milliseconds : %f \n", millisecs);

	return millisecs;

}

float MemoryBenchmarkRead()
{
	char *a;
	char *d_a;

	cudaEvent_t startT, stopT;
	float millisecs = 0;

	cudaEventCreate(&startT);
	cudaEventCreate(&stopT);

	a = (char *)malloc(MEMSIZE*sizeof(char));
	cudaMalloc(&d_a, MEMSIZE*sizeof(char));

	cudaEventRecord(startT);

	for (int i = 0; i < arguments.itrn; i++)
	{
		cudaMemcpy(a + i * (arguments.blockSize), d_a + i * (arguments.blockSize), arguments.blockSize * sizeof(char), cudaMemcpyDeviceToHost);
	}

	cudaEventRecord(stopT);

	cudaEventSynchronize(stopT);

	cudaEventElapsedTime(&millisecs, startT, stopT);

	printf("Time in milliseconds : %f \n", millisecs);

	return millisecs;

}

double ThroughputInGBps(float result, long blockSize, long itrn)
{
	return ((double)(blockSize*itrn) / (1000 * 1000* 1000)) / (((double)result / 1000));
}

void LatencyWrite()
{
	float result = MemoryBenchmarkWrite();
	printf("For Write :\nLatency : %.5g ms \t\t Throughput : %lf\tGBps\n", ((double)result / (double)arguments.itrn), ThroughputInGBps(result, arguments.blockSize, arguments.itrn));

}

void LatencyRead()
{
	float result = MemoryBenchmarkRead();
	printf("For Read :\nLatency : %.5g ms \t\t Throughput : %lf\tGBps\n", ((double)result / (double)arguments.itrn), ThroughputInGBps(result, arguments.blockSize, arguments.itrn));

}

__global__ void vectorADD(int *a, int *b, int *c)
{
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	if (i < SIZE)
	{
		a[i] = a[i] * 1 + 1 - 3 - 5 + 7 + 8 + 9;
		b[i] = b[i] * 1 + 1 - 3 - 5 + 7 + 8 + 9;
		c[i] = a[i] + b[i];
	}
}

__global__ void vectorFADD(float *a, float *b, float *c)
{
	int i = threadIdx.x; +blockIdx.x * blockDim.x;
	if (i < SIZE)
	{
		a[i] = a[i] + 1.23 + 2.03;
		b[i] = b[i] + 2.23 + 1.03;
		c[i] = a[i] + b[i];
		c[i] = c[i] * SIZE;
	}
}

void calcIOPs()
{
	int blockSize;
	int minGridSize;
	int threads;

	int *a, *b, *c;
	int *d_a, *d_b, *d_c;
	unsigned long long iop, iops;

	cudaEvent_t startT, stopT;
	float millisecs = 0;

	cudaEventCreate(&startT);
	cudaEventCreate(&stopT);


	a = (int *)malloc(SIZE * sizeof(int));
	b = (int *)malloc(SIZE * sizeof(int));
	c = (int *)malloc(SIZE * sizeof(int));

	cudaMalloc(&d_a, SIZE * sizeof(int));
	cudaMalloc(&d_b, SIZE * sizeof(int));
	cudaMalloc(&d_c, SIZE * sizeof(int));

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = 2;
		b[i] = 3;
		c[i] = 1;
	}

	cudaMemcpy(d_a, a, SIZE * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, SIZE * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_c, c, SIZE * sizeof(int), cudaMemcpyHostToDevice);

	cudaOccupancyMaxPotentialBlockSize(&minGridSize, &threads, vectorADD, 0, SIZE);
	blockSize = (SIZE + threads - 1) / threads;

	cudaEventRecord(startT);

	vectorADD << <blockSize, threads >> >(d_a, d_b, d_c);
	cudaEventRecord(stopT);

	cudaMemcpy(c, d_c, SIZE * sizeof(int), cudaMemcpyDeviceToHost);

	cudaEventSynchronize(stopT);

	cudaEventElapsedTime(&millisecs, startT, stopT);

	printf("Time in milliseconds : %f \n", millisecs);
	iop = SIZE * 16 * 1000;
	iops = iop / millisecs;
	printf("IOPS : %lu\n", iops);
	free(a);
	free(b);
	free(c);

	cudaFree(d_a);
	cudaFree(d_c);
	cudaFree(d_c);
}

void calcFLOPs()
{
	int threads;
	int minGridSize;
	int blocksize;

	float *a, *b, *c;
	float *d_a, *d_b, *d_c;
	unsigned long long flop, flops;

	cudaEvent_t startT, stopT;
	float millisecs = 0;

	cudaEventCreate(&startT);
	cudaEventCreate(&stopT);


	a = (float *)malloc(SIZE * sizeof(float));
	b = (float *)malloc(SIZE * sizeof(float));
	c = (float *)malloc(SIZE * sizeof(float));

	cudaMalloc(&d_a, SIZE * sizeof(float));
	cudaMalloc(&d_b, SIZE * sizeof(float));
	cudaMalloc(&d_c, SIZE * sizeof(float));

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = i;
		b[i] = i;
		c[i] = 0;
	}

	cudaMemcpy(d_a, a, SIZE * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, SIZE * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_c, c, SIZE * sizeof(float), cudaMemcpyHostToDevice);

	cudaOccupancyMaxPotentialBlockSize(&minGridSize, &threads, vectorFADD, 0, SIZE);
	blocksize = (SIZE + threads - 1) / threads;

	cudaEventRecord(startT);

	vectorFADD << <blocksize, threads >> >(d_a, d_b, d_c);
	cudaEventRecord(stopT);

	cudaMemcpy(c, d_c, SIZE * sizeof(float), cudaMemcpyDeviceToHost);

	cudaEventSynchronize(stopT);

	cudaEventElapsedTime(&millisecs, startT, stopT);

	printf("Time in milliseconds : %f \n", millisecs);
	flop = SIZE * 6 * 1000;
	flops = flop / millisecs;
	printf("FLOPS : %lu\n", flops);

	free(a);
	free(b);
	free(c);

	cudaFree(d_a);
	cudaFree(d_c);
	cudaFree(d_c);

}

__global__ void vectorADDS(int *a, int *b, int *c)
{
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = a[i] * 1 + 1 - 3 - 5 ;
		b[i] = b[i] * 1 + 1 - 3 - 5 ;
		c[i] = a[i] + b[i];
	}
}

__global__ void vectorFADDS(float *a, float *b, float *c)
{
	for (int i = 0; i < SIZE; i++)
	{
		//a[i] = a[i] + 1.23;
		//b[i] = b[i] + 2.23;
		c[i] = a[i] + b[i];
	}
}

void calcSTFLOPs()
{
	float *a, *b, *c;
	float *d_a, *d_b, *d_c;
	unsigned long long flop, flops;

	cudaEvent_t startT, stopT;
	float millisecs = 0;

	cudaEventCreate(&startT);
	cudaEventCreate(&stopT);


	a = (float *)malloc(SIZE * sizeof(float));
	b = (float *)malloc(SIZE * sizeof(float));
	c = (float *)malloc(SIZE * sizeof(float));

	cudaMalloc(&d_a, SIZE * sizeof(float));
	cudaMalloc(&d_b, SIZE * sizeof(float));
	cudaMalloc(&d_c, SIZE * sizeof(float));

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = 1.202;
		b[i] = 2.403;
		c[i] = 0;
	}

	cudaMemcpy(d_a, a, SIZE * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, SIZE * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_c, c, SIZE * sizeof(float), cudaMemcpyHostToDevice);

	cudaEventRecord(startT);

	vectorFADDS << <1,1 >> >(d_a, d_b, d_c);
	cudaEventRecord(stopT);

	cudaMemcpy(c, d_c, SIZE * sizeof(float), cudaMemcpyDeviceToHost);

	cudaEventSynchronize(stopT);

	cudaEventElapsedTime(&millisecs, startT, stopT);

	printf("Time in milliseconds : %f \n", millisecs);
	flop = SIZE * 1 *1000;
	flops = flop / millisecs;
	printf("FLOPS : %lu\n", flops);

	free(a);
	free(b);
	free(c);

	cudaFree(d_a);
	cudaFree(d_c);
	cudaFree(d_c);
}

void calcSTIOPs()
{
	int *a, *b, *c;
	int *d_a, *d_b, *d_c;
	unsigned long iop, iops;

	cudaEvent_t startT, stopT;
	float millisecs = 0;

	cudaEventCreate(&startT);
	cudaEventCreate(&stopT);


	a = (int *)malloc(SIZE * sizeof(int));
	b = (int *)malloc(SIZE * sizeof(int));
	c = (int *)malloc(SIZE * sizeof(int));

	cudaMalloc(&d_a, SIZE * sizeof(int));
	cudaMalloc(&d_b, SIZE * sizeof(int));
	cudaMalloc(&d_c, SIZE * sizeof(int));

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = 2;
		b[i] = 3;
		c[i] = 1;
	}

	cudaMemcpy(d_a, a, SIZE * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, SIZE * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_c, c, SIZE * sizeof(int), cudaMemcpyHostToDevice);

	cudaEventRecord(startT);

	vectorADD << <1, 1 >> >(d_a, d_b, d_c);
	cudaEventRecord(stopT);

	cudaMemcpy(c, d_c, SIZE * sizeof(int), cudaMemcpyDeviceToHost);

	cudaEventSynchronize(stopT);

	cudaEventElapsedTime(&millisecs, startT, stopT);

	printf("Time in milliseconds : %f \n", millisecs);
	iop = SIZE * 9 *1000;
	//millisecs to secs
	iops = iop / millisecs;
	printf("IOPS : %lu\n", iops);
	free(a);
	free(b);
	free(c);

	cudaFree(d_a);
	cudaFree(d_c);
	cudaFree(d_c);
}

int main(int argi, char *argc[])
{
	long blockSize = 0;
	char *operations;

	operations = argc[1];

	if (*operations == 's')
	{
		calcSTFLOPs();
		calcSTIOPs();
		
		return 0;
	}

	if (*operations == 'm')
	{
		calcIOPs();
		calcFLOPs();
		return 0;
	}

	blockSize = atol(argc[2]);

	arguments.blockSize = blockSize;
	long itrn = MEMSIZE/blockSize;
	arguments.itrn = itrn;

	if (*operations == 'w')
	{
		LatencyWrite();
		return 0;
	}
	if (*operations == 'r')
	{
		LatencyRead();
		return 0;
	}
	
	return 0;
}