#!/bin/bash

echo "Compiling..."
mkdir -p bin

cd bin

g++ -o midi-fi -DGL_GLEXT_PROTOTYPES -I../src/ -g ../src/*.cpp -lGL -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl || exit -1
echo "Compiled"
