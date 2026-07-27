//
// Created by Andrew Graser on 7/27/2026.
//


#pragma once

#include <iostream>

#include <cuda_runtime.h>

__global__
void Add(int n, float* x, float* y) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        y[i] = x[i] + y[i];
    }
}

void CudaAddTest(int threadCount) {
    int N = 1 << 20;

    float *x, *y;
    cudaMallocManaged(&x, N * sizeof(float));
    cudaMallocManaged(&y, N * sizeof(float));



    for (int i = 0; i < N; ++i) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    int blocks = (N + threadCount - 1) / threadCount;

    Add<<<blocks, threadCount>>>(N, x, y);

    cudaError_t launchError = cudaGetLastError();

    if (launchError != cudaSuccess)
    {
        std::cerr << "Kernel launch failed: "
                  << cudaGetErrorString(launchError)
                  << "\n";
    }


    cudaError_t err = cudaDeviceSynchronize();

    if(err != cudaSuccess)
    {
        std::cerr << cudaGetErrorString(err) << "\n";
    }

    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = fmax(maxError, fabs(y[i]-3.0f));
    std::cout << "Max error: " << maxError << std::endl;

    cudaFree(x);
    cudaFree(y);
}
