# shoulda

![shoulda checked the return value](shoulda.jpg)


Find function calls that do not use the return value:

http://stackoverflow.com/questions/43811955/how-can-i-lint-c-code-to-find-all-unused-return-values

## Build

1. Install dependencies

   Fedora:

        dnf install clang-devel llvm-devel llvm-static

2. Build with CMake

        mkdir build
        cd build
        cmake -G Ninja ..
        ninja

   (You can use Make or some other build system instead of Ninja.)
