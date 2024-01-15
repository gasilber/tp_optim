CC = gcc
CFLAGS = -O3 -fopenmp

transform_image: io.o transfo.o cycles.o
	$(CC) $(CFLAGS) -o $@ $^

io.o: io.c transfo.h cycles.h
	$(CC) $(CFLAGS) -c $< -o $@

transfo.o: transfo.c transfo.h
	$(CC) $(CFLAGS) -c $< -o $@

cycles.o: cycles.c cycles.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o transform_image
	rm -fr cmake-build-debug

