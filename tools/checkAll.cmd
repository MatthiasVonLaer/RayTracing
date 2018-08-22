ScriptRoot="$(cd "$(dirname "$0")" && pwd)"
$ScriptRoot/build.cmd && ctest --output-on-failure | grep --color -E '^|Failed'

