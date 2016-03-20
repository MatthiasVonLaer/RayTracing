Dependencies:

Qt5, Open MPI (and CMake for building)



Building:

mkdir bin/
cd bin/
cmake ../src/
make



Running the program:

Serial-mode:
./bin/raytracing  param/sample.scn

Parallel:
mpirun -np 5  ./bin/raytracing  param/sample.scn

Piped:
<e.g. physics_engine>  |  ./bin/raytracing


