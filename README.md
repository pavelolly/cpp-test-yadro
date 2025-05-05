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

This project uses `GTest` for unit-testing

By default building of unit tests is off. Enable it using the following command

```bash
cmake .. -DBUILD_UNIT_TESTS=ON
```

You might also want to help `cmake` find your `GTest`:

```bash
cmake .. -DBUILD_UNIT_TESTS=ON -DCMAKE_PREFIX_PATH="path/to/gtest"
```

You can simply run tests with `ctest` after building

This will install an executable and test files with input and output data for the program

```bash
cmake --install .
```


# Usage

```bash
cpp-test-yadro <file.txt>
```

You can test program with files from test folder. For Example

```bash
cpp-test-yadro data/001.dat | diff -b - data/001.ans
```

If output is clean then output of the program matches with the `.ans` file