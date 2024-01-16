#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "transfo.h"
#ifdef USE_CLOCK
#include <time.h>
#else
#include "cycles.h"
#endif

void read_look_up_table(FILE *map, unsigned char *lut)
{
	int n = 0;
	unsigned char val;
	while (fread(&val, 1, 1, map) != 0) {
		lut[n] = val;
		n++;
	}
}

void read_image(FILE *in, unsigned char *source_image, long size)
{
	long i;
	fseek(in, 1, SEEK_CUR);
	for (i = 0; i < size; i++) {
		if (fread(source_image + i, 1, 1, in) == 0) {
			perror("fread");
			exit(1);
		}
	}
}

void write_image(char *dest, unsigned char *img, long size, int width, int height, int maxval)
{
	FILE *out;
	long i;

	out = fopen(dest, "w");
	if (out == NULL)
	{
		perror(dest);
		exit(EXIT_FAILURE);
	}
	fprintf(out, "P5\n");
	fprintf(out, "%d %d\n", height, width);
	fprintf(out, "%d\n", maxval);
	for (i = 0; i < size; i ++) {
		if (fwrite (img + i, 1, 1, out) == EOF) {
			perror ("fwrite");
			exit (1);
		}
	}
	fclose (out);
}

#ifdef USE_CLOCK
clock_t transform_image(char *source, char *curve, int light, char *dest)
#else
double transform_image(char *source, char *curve, int light, char *dest)
#endif	
{
	FILE *in;
	FILE *map;
	char c1;
	char c2;
	int height;
	int width;
	int maxval;
	long size;
	unsigned char *source_image;
	unsigned char *dest_image;
	unsigned char *lut;
#ifdef USE_CLOCK
	clock_t start;
	clock_t stop;
	clock_t t;
#else
	double t;
#endif

	in = fopen(source, "r");
	if (in == NULL) {
		perror(source);
		exit(EXIT_FAILURE);
	}

	map = fopen(curve, "r");
	if (map == NULL) {
		perror(curve);
		exit(EXIT_FAILURE);
	}

	fscanf(in, "%c", &c1);
	fscanf(in, "%c", &c2);
	if (c1 != 'P' || c2 != '5') {
		fprintf(stderr, "Error, input file is not PGM\n");
		exit(EXIT_FAILURE);
	}

	fscanf (in, "%d %d", &height, &width);
	fscanf (in, "%d", &maxval);

	if (maxval > 255) {
		fprintf(stderr, "Input file is not a 8 bits gray image.\n");
		exit(EXIT_FAILURE);
	}

	size = width * height;
	printf ("%s: %d x %d = %ld pixels\n", source, width, height, size);

	/*
	 * Allocate memory for images and look up table
	 */
	source_image = (unsigned char *)malloc(sizeof(unsigned char) * size);
	if (source_image == NULL) {
		perror ("malloc for source image");
		exit (1);
	}
	dest_image = (unsigned char *)malloc(sizeof(unsigned char) * size);
	if (dest_image == NULL) {
		perror ("malloc for destination image");
		exit (1);
	}
	lut = (unsigned char *)malloc(sizeof(unsigned char) * 256);
	if (lut == NULL) {
		perror ("malloc for lut");
		exit (1);
	}

	read_look_up_table(map, lut);

	read_image(in, source_image, size);
	fclose (in);

#ifdef USE_CLOCK
	start = clock();
#else
	start_counter();
#endif
	transfo (width, height, source_image, dest_image, lut, light);
#ifdef USE_CLOCK
	stop = clock();
	t = stop - start;
	printf("%ld clock cycles.\n", t);
#else
	t = get_counter();
	printf("%f clock cycles.\n", t);
#endif
	write_image(dest, dest_image, size, width, height, maxval);

	free(source_image);
	free(lut);
	free(dest_image);

	return t;
}

#define FNMAX 255

void run_transfo_file(FILE *tf)
{
	char source[FNMAX];
	char curve[FNMAX];
	char dest[FNMAX];
	int light;
#ifdef USE_CLOCK
	clock_t total = 0;
#else
	double total = 0;
#endif

	// while (fscanf(tf, "%s %s %d %s", source, curve, &light, dest) == 4) {
	// 	printf("%s %s %d %s\n", source, curve, light, dest);
	// 	total += transform_image(source, curve, light, dest);
	// }
	
    // int total = 0;
    pid_t pid;
	while (fscanf(tf, "%s %s %d %s", source, curve, &light, dest) == 4) {
        printf("%s %s %d %s\n", source, curve, light, dest);

        pid = fork();

        if (pid == -1) {
            perror("Error in fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            int result = transform_image(source, curve, light, dest);
            exit(result);  // Exit with the result of the transformation
        }
    }

	if (pid != 0) { // Parent process
		while (pid != wait(0)) {
			// int status;
			// waitpid(pid, &status, 0);
			// total += WEXITSTATUS(status);  // Retrieve the exit status of the child process
		}
	}

#ifdef USE_CLOCK
	printf("TOTAL: %ld clock cycles.\n", total);
#else
	printf("TOTAL: %f clock cycles.\n", total);
#endif
}

int main (int ac, char *av[])
{
	FILE *tf;
	char *dname;

	if (ac != 2) {
		printf("Usage: %s transfofile\n", av[0]);
		exit(1);
	}

	tf = fopen(av[1], "r");
	if (tf == NULL) {
		perror(av[1]);
		exit(EXIT_FAILURE);
	}

	dname = dirname(av[1]);

	if (chdir(dname) == -1) {
		perror(dname);
		exit(EXIT_FAILURE);
	}

	run_transfo_file(tf);

	fclose(tf);

	return 0;
}
