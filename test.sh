#!/usr/bin/env sh

runTest()
{
    if [[ $(printf "${2}" | ./shell) = $3 ]]
    then
        echo "TEST $1: PASSED"
    else
        echo "TEST $1: FAILED"
    fi
}

# STAGE 1

runTest "0" "\n" ">>Exiting..."

runTest "1" "exit\n this should be ignored" ">Exiting..."

runTest "2" "" ">Exiting..."
