cmake --build . -- -j8 && ctest --output-on-failure | grep --color -E '^|Failed'
