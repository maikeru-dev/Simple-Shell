#!/usr/bin/env sh

runTest()
{
    if [[ $(echo $2 | ./shell) = $3 ]]
    then
        echo "TEST $1: PASSED"
    else
        echo "TEST $1: FAILED"
    fi
}

# STAGE 1

runTest "0" "\n" ">>"

runTest "1" "exit\n this should be ignored" ">"

runTest "2" "" ">"
