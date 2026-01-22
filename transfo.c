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

/* Experimental feature */
void sharpen(int w, int h, unsigned char *src, unsigned char *dest)
{
    int i, j;
    int val;

    /* Sharpen kernel: TODO: should be a parameter...
     *   0  -1  0
     *  -1   5 -1
     *   0  -1  0
     */

    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            if (i == 0 || i == w - 1 || j == 0 || j == h - 1) {
                /* Border pixels: copy as-is */
                dest[j * w + i] = src[j * w + i];
            } else {
                /* Apply 3x3 sharpen convolution */
                val = 5 * src[j * w + i]
                    - src[(j - 1) * w + i]
                    - src[(j + 1) * w + i]
                    - src[j * w + (i - 1)]
                    - src[j * w + (i + 1)];

                /* Clamp to [0, 255] */
                if (val < 0) {
                    val = 0;
                } else if (val > 255) {
                    val = 255;
                }
                dest[j * w + i] = (unsigned char) val;
            }
        }
    }
}

void transfo(int w, int h, unsigned char *src,
    unsigned char *dest, unsigned char *lut, unsigned char val)
{
    copy(w, h, src, dest);
    /* sharpen(w, h, src, dest); */ /* Experimental */
    curve(w, h, dest, lut);
    light(w, h, dest, val);
}
