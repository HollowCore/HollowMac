# HollowMac
The macOS specific parts of HollowMac.

[![Build Status](https://travis-ci.org/HollowMac/HollowMac.svg?branch=master)](https://travis-ci.org/HollowMac/HollowMac)

# Build Requirements
```bash
$ sudo apt install build-essential cmake git
```

# Build
* Requires cmake and C compiler

```bash
$ cd HollowMac
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Test
* Requires cmake and C compiler

```bash
$ cd HollowMac
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test
$ ctest --verbose
```

# Memory Test Requirements
```bash
$ sudo apt install build-essential cmake git valgrind
```

# Memory Testing
```bash
$ cd HollowMac
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test
$ valgrind ./hollowmactest
```

# Code Coverage
* Requires cmake, gcc or llvm, gcov, lcov, genhtml

```bash
$ cd HollowMac
$ mkdir build
$ cd build
$ cmake .. -DCOVERAGE=1
$ make coverage
$ open coverage/html/index.html
```
