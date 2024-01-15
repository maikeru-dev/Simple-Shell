#!/bin/bash

gcc -Wall ./src/* -o ./out/* && ./test.sh && ln -s ./out/shell shell
