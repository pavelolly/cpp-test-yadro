# Description

This project exists as part of a test assignment for an internship.

The program reads files in a specific format and outputs some processed information.

To learn more about the formats visit `tests` folder with `*.dat` and `*.ans` files

# Build

Use regular `cmake` building process to build program

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The program was tested to build with GCC/Clang on Linux and MSVC on Windows.

This project uses `GTest` for unit-testing

By default unit tests building is disabled. You can enable it using the following option

```bash
cmake .. -DBUILD_UNIT_TESTS=ON
```

You might also want to help `cmake` find your `GTest`:

```bash
cmake .. -DBUILD_UNIT_TESTS=ON -DCMAKE_PREFIX_PATH="path/to/gtest"
```

`ctest` runs unit tests

`cmake --install .` installs the executable and test files with input and output data for the program

# Usage

```bash
cpp-test-yadro <file.txt>
```

You can test program with files from test folder. For Example

```bash
cpp-test-yadro data/001.dat | diff -w - data/001.ans
```

No output from the above command and exit code 0 mean that output of the program matches the corresponding `.ans` file