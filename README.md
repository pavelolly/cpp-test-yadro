# Description

This project exists as part of a test assignment for an internship.

The program reads files in a specific format and outputs some processed information.

To learn more about the formats visit `tests` folder with `*.dat` and `*ans` files

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

# Usage

```bash
cpp-test-yadro <file.txt>
```