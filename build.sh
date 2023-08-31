#!/bin/bash

set -xe

gcc -O3 -Wall -Wextra main.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11