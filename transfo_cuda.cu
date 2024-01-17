#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "transfo_cuda.h"

#define DIM 256

__global__ void copyKernel(int w, int h, unsigned char *src, unsigned char *dest)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < w && j < h)
    {
        dest[j * w + i] = src[j * w + i];
    }
}

extern "C" void copyGPU(int w, int h, unsigned char *src, unsigned char *dest)
{
    // Define block and grid dimensions
    dim3 blockDim(DIM, DIM); // Adjust according to your GPU architecture
    dim3 gridDim((w + blockDim.x - 1) / blockDim.x, (h + blockDim.y - 1) / blockDim.y);

    // Allocate device memory
    unsigned char *d_src, *d_dest;
    cudaMalloc((void **)&d_src, w * h * sizeof(unsigned char));
    cudaMalloc((void **)&d_dest, w * h * sizeof(unsigned char));

    // Copy input data from host to device
    cudaMemcpy(d_src, src, w * h * sizeof(unsigned char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_dest, dest, w * h * sizeof(unsigned char), cudaMemcpyHostToDevice);


    // Launch the kernel
    copyKernel<<<gridDim, blockDim>>>(w, h, d_src, d_dest);

    // Copy the result back to the host
    cudaMemcpy(dest, d_dest, w * h * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_src);
    cudaFree(d_dest);
}

__global__ void lightKernel(int w, int h, unsigned char *img, unsigned char val)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < w && j < h)
    {
        unsigned char current = img[j * w + i];
        img[j * w + i] = ((int)current + val > 255) ? 255 : current + val;
    }
}

extern "C" void lightGPU(int w, int h, unsigned char *img, unsigned char val)
{
    dim3 blockDim(DIM, DIM); // Adjust according to your GPU architecture
    dim3 gridDim((w + blockDim.x - 1) / blockDim.x, (h + blockDim.y - 1) / blockDim.y);

    // Allocate device memory
    unsigned char *d_img;
    cudaMalloc((void **)&d_img, w * h * sizeof(unsigned char));

    // Copy input data from host to device
    cudaMemcpy(d_img, img, w * h * sizeof(unsigned char), cudaMemcpyHostToDevice);

    lightKernel<<<gridDim, blockDim>>>(w, h, d_img, val);

    // Copy the result back to the host
    cudaMemcpy(img, d_img, w * h * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_img);
}

__global__ void curveKernel(int w, int h, unsigned char *img, unsigned char *lut)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < w && j < h)
    {
        unsigned char current = img[j * w + i];
        img[j * w + i] = lut[current];
    }
}

extern "C" void curveGPU(int w, int h, unsigned char *img, unsigned char *lut)
{
    dim3 blockDim(DIM, DIM); // Adjust according to your GPU architecture
    dim3 gridDim((w + blockDim.x - 1) / blockDim.x, (h + blockDim.y - 1) / blockDim.y);

    // Allocate device memory
    unsigned char *d_img, *d_lut;
    cudaMalloc((void **)&d_img, w * h * sizeof(unsigned char));
    cudaMalloc((void **)&d_lut, w * h * sizeof(unsigned char));

    // Copy input data from host to device
    cudaMemcpy(d_img, img, w * h * sizeof(unsigned char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_lut, lut, w * h * sizeof(unsigned char), cudaMemcpyHostToDevice);

    // Launch the kernel
    curveKernel<<<gridDim, blockDim>>>(w, h, d_img, d_lut);

    // Copy the result back to the host
    cudaMemcpy(img, d_img, w * h * sizeof(unsigned char), cudaMemcpyDeviceToHost);
    cudaMemcpy(lut, d_lut, w * h * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_img);
    cudaFree(d_lut);
}