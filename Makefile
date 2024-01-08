#CFLAGS = -O3
transform_image: io.o transfo.o cycles.o
	$(CC) -o $@ $^
io.o: transfo.h cycles.h
clean:
	rm -f *.o transform_image
	rm -fr cmake-build-debug

