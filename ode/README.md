
```
# uncompress the .tar.gz then try to compile:
gcc-11 -fopenmp -I./roadrunner/include/rr/C -L./roadrunner/lib -lroadrunner_c_api test_omp_rr.c  -o test_omp_rr

# tell the executable where to find the shared libs, e.g.
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:./roadrunner/lib
```

Try to run:
```
test_omp_rr 1 1
```

