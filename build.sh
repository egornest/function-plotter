#!/bin/sh
set -e
rm -f *.o *.exe

CFLAGS="-g -std=c++17 -Wall -Wextra -Werror -pedantic -O2"

g++ $CFLAGS -c main.cpp -o main.o
g++ $CFLAGS -c parser.cpp -o parser.o
g++ $CFLAGS -c funcbuilder.cpp -o funcbuilder.o
g++ $CFLAGS -c draw.cpp -o draw.o
g++ $CFLAGS draw.o main.o parser.o funcbuilder.o -o func_plotter.exe -lcairo