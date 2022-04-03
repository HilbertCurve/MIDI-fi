#!/bin/bash

echo "Running tests..."
echo ""

mkdir -p bin

cd bin

failed=0

function run_test () {
    g++ -o $1 -DGL_GLEXT_PROTOTYPES -I../src/ -g $2 ../tests/$3 || { 
        echo "$3 failed to compile." && failed=1 && return -1 
    }
    echo "$3 compiled successfully."

    ./$1 || { 
        echo "$3 failed to run." && failed=1 && return -2 
    }
    echo "$3 ran successfully."
    return 0
}

run_test memory-test "../src/Memory.cpp" "../tests/MemoryTests.cpp"
run_test arena-test "../src/Memory.cpp" "../tests/ArenaTests.cpp"
run_test string-test "../src/Utils.cpp ../src/Memory.cpp" "../tests/StringTests.cpp"

echo ""
if [ $failed == 0 ]; then 
    echo "All tests ran successfully."
fi

