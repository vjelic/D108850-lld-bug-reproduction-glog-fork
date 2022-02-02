#!/bin/bash

declare -g LLVM_BUILD_ROOT="$HOME/llvm-project/build"
declare -g REPRODUCER_ROOT="$HOME/lld-reproducer"

pushd "$LLVM_BUILD_ROOT"
ninja lld
LLVM_BUILD_STATUS=$?
popd
if [[ $LLVM_BUILD_STATUS != 0 ]]; then
    exit 125
fi

pushd "$REPRODUCER_ROOT"
cmake -G Ninja -B ./build . "-DLLVM_ROOT=${LLVM_BUILD_ROOT}"
CMAKE_STATUS=$?
if [[ $CMAKE_STATUS != 0 ]]; then
    popd
    exit 125
fi
pushd build

ninja clean
ninja
NINJA_STATUS=$?
if [[ $NINJA_STATUS != 0 ]]; then
    popd
    popd
    exit 125
fi

./wrapper ../kernel1.o ../kernel2.o
WRAPPER_STATUS=$?

popd
popd
exit $WRAPPER_STATUS
