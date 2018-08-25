#!/bin/bash
nCores=$(nproc --all)
cmake --build . --config Release -- -j$nCores \
        && ctest --output-on-failure | grep --color -E '^|Failed'
