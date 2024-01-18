NVCC = nvcc
CC = gcc
CFLAGS = -O3
CUDAFLAGS = -arch=sm_86  # Adjust according to your GPU architecture

transform_image: io.o transfo.o transfo_cuda.o cycles.o
	$(NVCC) $(CUDAFLAGS) -o $@ $^

io.o: io.c transfo.h cycles.h
	$(CC) $(CFLAGS) -c $< -o $@

transfo.o: transfo.c transfo.h transfo_cuda.h
	$(NVCC) $(CUDAFLAGS) -c $< -o $@

transfo_cuda.o: transfo_cuda.cu transfo_cuda.h
	$(NVCC) $(CUDAFLAGS) -c $< -o $@

cycles.o: cycles.c cycles.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o transform_image
	rm -fr cmake-build-debug

