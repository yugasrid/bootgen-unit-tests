# Bootgen Unit Tests

This directory contains unit tests for the Bootgen application's main functionality.

## Overview

The unit tests cover the following areas:
- `BootGenApp` class functionality
- Command-line argument parsing
- Exception handling
- BIF file processing workflow
- Performance and memory usage
- Banner display

## Test Files

1. **test_main.cpp** - Comprehensive tests using Google Test framework
2. **test_main_simple.cpp** - Self-contained tests with minimal dependencies
3. **CMakeLists.txt** - CMake build configuration
4. **Makefile** - Traditional make build configuration
5. **run_tests.bat** - Windows batch script for easy test execution

## Running the Tests

### Option 1: Using the Windows Batch Script (Recommended for Windows)

```batch
cd tests
run_tests.bat
```

This script will:
- Check for required files
- Attempt to compile using available compilers (g++ or Visual Studio cl)
- Run the compiled tests
- Display results

### Option 2: Using Make (Linux/Unix/MSYS2)

```bash
cd tests
make test
```

### Option 3: Using CMake

```bash
cd tests
mkdir build
cd build
cmake ..
make
./bootgen_tests
```

### Option 4: Manual Compilation

#### With g++ (MinGW/MSYS2/Linux):
```bash
g++ -std=c++11 -I../common/include -I../spartanup/include -I../versal/include -I../versal_2ve_2vm/include -I../zynq/include -I../zynqmp/include -I../utils/include -I../lms-hash-sigs -I../win_include -I../bisonflex -o test_runner test_main_simple.cpp
./test_runner
```

#### With Visual Studio cl:
```batch
cl /EHsc /I..\common\include /I..\spartanup\include /I..\versal\include /I..\versal_2ve_2vm\include /I..\zynq\include /I..\zynqmp\include /I..\utils\include /I..\lms-hash-sigs /I..\win_include /I..\bisonflex test_main_simple.cpp /Fe:test_runner.exe
test_runner.exe
```

## Test Structure

### Test Categories

1. **Functional Tests**
   - `BootGenApp_Run_WithValidBifFile` - Tests normal execution flow
   - `BootGenApp_Run_WithEmptyBifFile` - Tests handling of empty BIF filename
   - `BootGenApp_Run_CallsRequiredMethods` - Verifies method call sequence

2. **Exception Handling Tests**
   - `ExceptionHandling_StdException` - Tests std::exception catching
   - `ExceptionHandling_CharPointer` - Tests const char* exception catching
   - `ExceptionHandling_UnknownException` - Tests catch-all exception handling

3. **Argument Parsing Tests**
   - `ArgumentParsing_NoArguments` - Tests behavior with no arguments
   - `ArgumentParsing_ImageArgument` - Tests -image argument parsing
   - `ArgumentParsing_MultipleArguments` - Tests complex argument combinations

4. **Performance Tests**
   - `Performance_QuickExecution` - Ensures reasonable execution time
   - `Memory_NoLeaks` - Basic memory leak detection

5. **Integration Tests**
   - `MainFunction_Simulation_Success` - Simulates main function behavior
   - `BIF_File_Processing` - Tests BIF file handling
   - `Options_DefaultState` - Tests Options class initial state

## Expected Test Results

When all tests pass, you should see output similar to:

```
Bootgen Unit Tests
Copyright 2023 Advanced Micro Devices, Inc.

Running 15 tests...
========================================
Running: BootGenApp_Run_WithValidBifFile... [PASS]
Running: BootGenApp_Run_WithEmptyBifFile... [PASS]
Running: BootGenApp_Run_CallsRequiredMethods... [PASS]
...
========================================
Tests completed: 15
Passed: 15
Failed: 0
All tests passed!
```

## Dependencies

### For test_main_simple.cpp (Recommended):
- C++11 compatible compiler
- Standard C++ library
- No external dependencies

### For test_main.cpp (Advanced):
- Google Test framework
- Google Mock framework
- All dependencies from the main application

## Troubleshooting

### Common Issues:

1. **Compiler not found**
   - Install MinGW/MSYS2 for g++
   - Install Visual Studio Build Tools for cl
   - Ensure compiler is in PATH

2. **Header files not found**
   - Verify you're running from the tests directory
   - Check that ../common/include/ exists and contains required headers

3. **Linking errors**
   - For full integration tests, you may need to link against the main application's object files
   - The simple tests avoid this by using mock implementations

### For Google Test Setup:

#### Ubuntu/Debian:
```bash
sudo apt-get install libgtest-dev libgmock-dev
```

#### Windows (vcpkg):
```batch
vcpkg install gtest gmock
```

## Adding New Tests

To add new test cases:

1. **For simple tests** (test_main_simple.cpp):
   - Create a new function following the pattern `bool test_YourTestName()`
   - Use TEST_ASSERT macros for assertions
   - Add the test to the runner in main()

2. **For Google Test** (test_main.cpp):
   - Use TEST() or TEST_F() macros
   - Follow Google Test conventions

## Integration with CI/CD

The tests can be integrated into automated build systems:

```bash
# Example CI script
cd tests
make test
if [ $? -eq 0 ]; then
    echo "All tests passed"
else
    echo "Tests failed"
    exit 1
fi
```

## Notes

- The simple test framework is designed to work without external dependencies
- Tests use mock implementations to avoid dependency on actual file I/O
- Performance tests have reasonable timeouts to detect infinite loops or hangs
- Memory tests perform basic leak detection through repeated execution
