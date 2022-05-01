#!/bin/bash

if [ $(id -u) -ne 0 ]
  then echo "Please run as root, we will need the make install command"
  exit
fi

cd ../../
mkdir ./FiltFilt-build
cd ./FiltFilt-build
cmake -S ../FiltFilt/src -B ./
cmake --build ./ -j
make install