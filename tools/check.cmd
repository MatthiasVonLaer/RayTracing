ScriptRoot="$(cd "$(dirname "$0")" && pwd)"
$ScriptRoot/build.cmd && ctest -R raytracing_unit_test --output-on-failure | grep --color -E '^|Failed'
