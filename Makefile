CFLAGS = -Wall -O3 -fopenmp
run: transform_image
	time ./transform_image ../data/transfo.txt
transform_image: io.o transfo.o cycles.o
	${CC} ${CFLAGS} -o $@ $^
io.o: transfo.h cycles.h
clean:
	rm -f *.o transform_image
	rm -fr cmake-build-debug
check:
	diff ../data/image1_t.pgm ../data/references/res1.pgm
