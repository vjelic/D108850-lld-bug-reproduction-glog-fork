# Bug when lld invoked multiple times

To reproduce (with a build of LLVM that includes lld)

```
mkdir build
cd build
cmake -G Ninja .. -DLLVM_ROOT=/your/llvm-project/build
./wrapper ../kernel1.o
./wrapper ../kernel2.o
.wrapper ../kernel1.o ../kernel2.o
```

Note that while the first two invocations of wrapper and the last one
both make the same invocations of `lld::elf::link`, the one with two calls in the
same process fails.

This used to work fine so there's a bug, and the bisection log at
[bisect-log](./bisect-log) is
```
[83d59e05b201760e3f364ff6316301d347cbad95] Re-land [LLD] Remove global state in lldCommon
```
