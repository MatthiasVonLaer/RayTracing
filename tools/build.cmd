nCores=$(nproc --all)
cmake --build . -- -j$nCores
