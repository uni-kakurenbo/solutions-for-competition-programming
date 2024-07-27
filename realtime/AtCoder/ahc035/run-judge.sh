#!/bin/bash

EXECUTION_COMMAND="$1"
CASE_ID="cases100"
PARALLEL=10

if [ "$2" != "" ]; then
    CASE_ID="$2"
fi

if [ "$3" != "" ]; then
    PARALLEL="$3"
fi

echo "Parallel execution: $PARALLEL"

function run_case() {
    local case="$1"
    local executin_command="$2"
    local file="$3"

    # shellcheck disable=SC2094
    case_name=$(basename "$file")
    case_name="${case_name%.*}"

    echo -n "$case_name: "

    local started_at ended_at
    started_at=$(date +%s.%N)

    # shellcheck disable=SC2086
    timeout 4 cargo run --release --bin tester $executin_command <"$file" >"$case/$case_name.res" 2>"$case/$case_name.log"
    cp "$case/$case_name.res" "out/$case_name.txt"

    echo $? >"$case/$case_name.info"

    ended_at=$(date +%s.%N)

    local execute_time
    execute_time=$(echo "($ended_at- $started_at) * 1000" | bc)

    echo "$execute_time" >>"$case/$case_name.info"
}

export -f run_case

find "$CASE_ID" -name "*.txt" -print0 | xargs -0 -P "$PARALLEL" -I {} bash -c "run_case '$CASE_ID' '$EXECUTION_COMMAND' {}"
echo
./calc-score.sh "$CASE_ID"
