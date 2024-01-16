void copy (int w, int h, unsigned char *src, unsigned char *dest)
{
	int i,j;

  	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			dest[j * w + i] = src[j * w + i];
		}
	}
}

void light(int w, int h, unsigned char *img, unsigned char val)
{
	int i,j;
	unsigned char current;

	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			current = img[j * w + i];
			img[j * w + i] = (((int) current + val) > 255) ? 255 : current + val;
		}
	}
}

void curve(int w, int h, unsigned char *img, unsigned char *lut)
{
	int i,j;
  	unsigned char current;

  	for (i = 0; i < w; i++) {
  		for (j = 0; j < h; j++) {
  			current = img[j * w + i];
			img[j * w + i] = lut[current];
  		}
  	}
}

void transfo(int w, int h, unsigned char *src,
             unsigned char *dest, unsigned char *lut, unsigned char val)
{
  	// copy(w, h, src, dest);
  	// curve(w, h, dest, lut);
  	// light(w, h, dest, val);
	register int i,j;
  	register unsigned char* current;

  	for (i = 0; i < w; i++) {
		for (j = 0; j < h; j++) {
			// dest[j * w + i] = src[j * w + i];
  			// current = dest[j * w + i];
			// dest[j * w + i] = lut[current];
			// current = dest[j * w + i];
			current = &lut[src[j * w + i]];
			// dest[j * w + i] = (((int) current + val) > 255) ? 255 : current + val;
			if (((short) *current + val) > 255)
				dest[j * w + i] = 255;
			else
				dest[j * w + i] = *current + val;
		}
	}
}
