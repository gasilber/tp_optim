static unsigned cyc_hi = 0;
static unsigned cyc_lo = 0;

void access_counter (unsigned *hi, unsigned *lo)
{
	asm ("rdtsc; movl %%edx,%0; movl %%eax,%1"
	: "=r" (*hi), "=r" (*lo)
	:
	: "%edx", "%eax");
}

void start_counter ()
{
	access_counter (&cyc_hi, &cyc_lo);
}

double get_counter ()
{
	unsigned ncyc_hi, ncyc_lo;
	unsigned hi, lo, borrow;

	access_counter (&ncyc_hi, &ncyc_lo);

	lo = ncyc_lo - cyc_lo;
	borrow = lo > ncyc_lo;
	hi = ncyc_hi - cyc_hi - borrow;
	return (double) hi * (1 << 30) * 4 + lo;
}

