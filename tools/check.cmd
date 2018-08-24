nCores=$(nproc --all)
cmake --build . --config release --target raytracing_unit_test -- -j$nCores \
        && ctest -R raytracing_unit_test --output-on-failure | grep --color -E '^|Failed'
