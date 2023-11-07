module purge
module load intel/2021a

make clean
make

mpirun -n 64 ./rc_run 64 100000