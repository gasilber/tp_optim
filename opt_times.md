# TP OPTIMISATION - ADRIAN RAMIREZ
- Commits reflect the different techniques of optimization (I implemented 2)
- I focused on speeding up each function (I did not try to mix them into 1)
- Best optimization occurs when using multi-threading with OpenMP (x6.73 faster)
- Optimizing with GPU did not speed up as much as expected (x4.83)
    - I have to admit that I did not go to deep into CUDA programming, just the fact of compiling 
    CUDA files and C files was tedious. 

# BASELINE
TOTAL: 5758619200.000000 clock cycles.
TOTAL: 5777182718.000000 clock cycles.
TOTAL: 5813660024.000000 clock cycles.

# MULTITHREAD - OpenMP
TOTAL: 855269062.000000 clock cycles.
TOTAL: 856301968.000000 clock cycles.
TOTAL: 858284726.000000 clock cycles.

# GPU - Nvidia Cuda Toolkit
TOTAL: 1187945424.000000 clock cycles.
TOTAL: 1180364952.000000 clock cycles.
TOTAL: 1226940368.000000 clock cycles.


