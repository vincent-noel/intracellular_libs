
```
gcc-11 -fopenmp -I/Users/heiland/git/intracellular_libs_arm64/ode/include/rr -L/Users/heiland/dev/roadrunner/build-release2/lib -lroadrunner_c_api  test_omp_rr.c -o test_omp_rr

gcc-11 -fopenmp -I./include/rr -L./libs/macos12_arm64 -lroadrunner_c_api  test_omp_rr.c -o test_omp_rr

export DYLD_LIBRARY_PATH=<full-path-to>/libs/<platform>
e.g.,
export DYLD_LIBRARY_PATH=./libs/macos12_arm64

Try to run:
test_omp_rr
---

