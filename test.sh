#!/bin/bash

mkdir -p bin

cd bin

failed=0
do_tests=0

if [ $# -gt 0 ] && [ ${0}=="-t" ]; then
    do_tests=1
fi

if [ ${do_tests} == 1 ]; then

echo "[SCRIPT] Running tests..."
echo ""

function run_test () {
    g++ -o $1 -DGL_GLEXT_PROTOTYPES -I../src/ -g $2 ../tests/$3 || { 
        echo "[SCRIPT] $3 failed to compile." && failed=1 && return -1 
    }
    echo "[SCRIPT] $3 compiled successfully."

    ./$1 || { 
        echo "[SCRIPT] $3 failed to run." && failed=1 && return -2 
    }
    echo "[SCRIPT] $3 ran successfully."
    return 0
}

run_test memory-test "../src/Memory.cpp" "../tests/MemoryTests.cpp"
run_test arena-test "../src/Memory.cpp" "../tests/ArenaTests.cpp"
run_test string-test "../src/Utils.cpp ../src/Memory.cpp" "../tests/StringTests.cpp"

echo ""
if [ $failed == 0 ]; then 
    echo "[SCRIPT] All tests ran successfully."
else
    exit -1
fi
echo ""

fi # if [ !do_tests ]; then

cd ..

echo "[SCRIPT] Running MIDI-fi..."
echo ""

./build.sh || {
    echo "[SCRIPT] MIDI-fi failed to compile."
    exit -1
}

echo ""
echo "[SCRIPT] MIDI-fi compiled successfully."
echo ""

./bin/midi-fi || {
    echo "[SCRIPT] MIDI-fi failed to run."
    exit -1
}

echo ""
echo "[SCRIPT] MIDI-fi ran successfully."

