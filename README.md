# TP Optim

Subject: https://www.cri.ensmp.fr/people/silber/para/

## Hardware

OVH Cloud AI Notebook:
- CPU : 13 vCores (Intel(R) Xeon(R) Gold 6226R CPU @ 2.90GHz)
- RAM : 40 Gio

## Tests

Please find the outputs of each test in perf_XX_*.txt, including time information.

- initial time: 14s
- add compiler optimization flags: negligeable
- remove loops: 3s gain
- locality: negligeable
- omp: 2s gain, no more gain after 7 cores
- fork: 13s gain: it is going very fast, less than one second for all the images

Unfortunately, I could not install `perf` in the AI notebook to do further tests.

What I find weird is that we can sometimes find more clock cycles but a shorter user time (see for instance `perf_00` and `perf_01`). It is also weird that for OpenMP, real is lower than user...

The loop improvement significantly decreases the number of clock cycles per image. Adding OpenMP increases the number of clock cycles per image but decreases the overall time thanks to parallel processing. 

I am also surprised to see that forking is much more efficient than OpenMP. Fork without OpenMP does not change the number of clock cycles per image though, compared to the non-OpenMP optimized version.

## Other resources

https://stackoverflow.com/questions/14492436/g-optimization-beyond-o3-ofast
https://stackoverflow.com/questions/16699247/what-is-a-cache-friendly-code

