#ifndef TRANSFO_CUDA_H
#define TRANSFO_CUDA_H

#ifdef __cplusplus
extern "C" {
#endif

void copyGPU (int w, int h, unsigned char *src, unsigned char *dest);
void lightGPU(int w, int h, unsigned char *img, unsigned char val);
void curveGPU(int w, int h, unsigned char *img, unsigned char *lut);

#ifdef __cplusplus
}
#endif

#endif