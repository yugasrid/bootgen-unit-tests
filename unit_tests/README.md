# Bootgen Unit Tests

This directory contains comprehensive unit tests for the Bootgen application, organized into separate test categories for better maintainability and reporting.

## Test Structure

The unit tests are organized as follows:

```
unit_tests/
├── test_framework.h           # Common test framework with assertion macros
├── test_framework.cpp         # Test framework implementation and reporting
├── mock_classes.h            # Mock classes for testing (simple & realistic)
├── test_basic_functionality.cpp      # Basic application functionality tests
├── test_argument_parsing.cpp          # Command-line argument parsing tests
├── test_exception_handling.cpp        # Exception handling and error cases
├── test_bif_file_processing.cpp       # BIF file processing tests
├── test_performance_memory.cpp        # Performance and memory management tests
├── test_rigorous_bug_detection.cpp    # Rigorous tests designed to find bugs
├── run_tests.ps1             # PowerShell test runner (Windows)
├── run_tests.sh              # Bash test runner (Linux/macOS)
└── test_reports/             # Generated test reports (created at runtime)
```

## Test Categories

### 1. Basic Functionality Tests (`test_basic_functionality.cpp`)
- Tests core application startup and execution
- Verifies basic command-line processing
- Ensures DisplayBanner functionality
- Tests with various argument combinations

### 2. Argument Parsing Tests (`test_argument_parsing.cpp`)
- Comprehensive testing of command-line argument parsing
- Tests various flag combinations (`-image`, `-o`, `-arch`, `-help`, `-verbose`)
- Edge cases with missing arguments
- Validation of parsed values

### 3. Exception Handling Tests (`test_exception_handling.cpp`)
- Tests all three exception types in main(): `std::exception`, `const char*`, and unknown
- Verifies proper return codes for different exception types
- Tests nested exception handling
- Resource cleanup during exceptions

### 4. BIF File Processing Tests (`test_bif_file_processing.cpp`)
- BIF file validation and processing
- Tests with valid, invalid, and edge-case filenames
- Error handling for malformed files
- Processing state verification

### 5. Performance and Memory Tests (`test_performance_memory.cpp`)
- Performance benchmarks for key operations
- Memory leak detection through repeated operations
- Stress testing with large datasets
- Resource usage validation

### 6. Rigorous Bug Detection Tests (`test_rigorous_bug_detection.cpp`)
- **Designed to find real bugs** - some tests may fail!
- Buffer overflow detection
- Null pointer dereference testing
- Memory management issues
- Input validation bypass attempts
- Resource exhaustion scenarios

## Test Framework Features

### Custom Assertion Macros
- `EXPECT_NO_THROW(statement)` - Verifies no exceptions are thrown
- `EXPECT_THROW(statement, exception_type)` - Verifies specific exception is thrown
- `EXPECT_EQ(expected, actual)` - Tests equality
- `EXPECT_TRUE(condition)` / `EXPECT_FALSE(condition)` - Boolean assertions
- `EXPECT_LT(val1, val2)` / `EXPECT_GT(val1, val2)` - Comparison assertions
- `EXPECT_STREQ(str1, str2)` / `EXPECT_STRNE(str1, str2)` - String comparisons
- `SUCCEED()` / `FAIL(message)` - Explicit pass/fail

### Reporting Features
- Individual test timing
- Detailed pass/fail status
- Exception capture and reporting
- Performance metrics
- Summary reports in text format
- Colored console output (PowerShell)

### Mock Classes
Two types of mock implementations:

#### Simple Mocks (`MockOptions`, `MockBIF_File`, `TestableBootGenApp`)
- Clean, predictable behavior for basic testing
- No real error conditions
- Good for testing happy paths

#### Realistic Mocks (`RealisticOptions`, `RealisticBIF_File`, `RealisticBootGenApp`)
- Contain intentional bugs and vulnerabilities
- Buffer overflow conditions
- Memory management issues
- Used for rigorous bug detection

## Building and Running Tests

### Build All Tests
```bash
make unit-tests
```

### Run All Tests with Comprehensive Reporting
```bash
make test-all
```

### Run Individual Test Categories
```bash
make test-basic          # Basic functionality
make test-args           # Argument parsing
make test-exceptions     # Exception handling
make test-bif            # BIF file processing
make test-performance    # Performance and memory
make test-rigorous       # Bug detection (may fail!)
```

### Manual Test Execution
```bash
# Build tests first
make unit-tests

# Run test runner script
cd unit_tests
powershell -ExecutionPolicy Bypass -File run_tests.ps1   # Windows
# OR
./run_tests.sh                                           # Linux/macOS

# Run individual test executable
../build/unit_tests/test_basic_functionality
```

## Test Reports

After running tests, reports are generated in `unit_tests/test_reports/`:

- `SUMMARY_REPORT.txt` - Overall test execution summary
- `basic_functionality_report.txt` - Basic functionality test details
- `argument_parsing_report.txt` - Argument parsing test details
- `exception_handling_report.txt` - Exception handling test details
- `bif_file_processing_report.txt` - BIF file processing test details
- `performance_memory_report.txt` - Performance and memory test details
- `rigorous_bug_detection_report.txt` - Bug detection test details

### Report Contents
Each report includes:
- Test execution timestamp
- Pass/fail counts and success rate
- Individual test results with timing
- Failed test details
- Performance metrics
- Error messages and exception details

## Understanding Test Results

### All Tests Pass ✅
This indicates either:
1. The code is very robust and well-tested
2. The tests might need to be more aggressive (especially for rigorous tests)

### Some Tests Fail ❌
This is **expected and good** for the rigorous bug detection tests! Failed tests indicate:
1. Real bugs that need fixing
2. Edge cases not properly handled
3. Security vulnerabilities
4. Memory management issues

### Rigorous Tests Specifically
The `test_rigorous_bug_detection` tests are **designed to fail** when they find bugs. This is the correct behavior - these tests expose real issues that should be addressed.

## Best Practices for Unit Testing

### What These Tests Demonstrate

1. **Test Real Error Conditions** - Don't just test happy paths
2. **Use Realistic Data** - Test with real-world inputs, not sanitized test data
3. **Test Boundaries** - Maximum values, empty inputs, null pointers
4. **Test Resource Limits** - Memory exhaustion, large datasets
5. **Test Security** - Input validation, buffer overflows, injection attempts
6. **Measure Performance** - Ensure operations complete within reasonable time
7. **Verify Cleanup** - Memory leaks, resource management

### Anti-Patterns Avoided

1. **Mock Everything** - Realistic implementations catch more bugs
2. **Only Happy Paths** - Error conditions are where bugs hide
3. **Ignore Performance** - Slow operations impact user experience
4. **No Resource Testing** - Memory leaks are common C++ issues
5. **Superficial Assertions** - Test behavior, not just API calls

## Integration with CI/CD

These tests can be integrated into continuous integration:

```bash
# In your CI script
make unit-tests
if make test-all; then
    echo "All tests passed"
else
    echo "Tests failed - check reports"
    exit 1
fi
```

## Adding New Tests

### To add a new test category:

1. Create `test_new_category.cpp` in `unit_tests/`
2. Include the test framework: `#include "test_framework.h"`
3. Include mock classes if needed: `#include "mock_classes.h"`
4. Write test functions
5. Add `RUN_TEST(test_function_name)` calls in main()
6. Add build target to Makefile
7. Update test runner scripts

### Test Function Template:
```cpp
void test_MyNewFeature() {
    // Setup
    MyClass obj;
    
    // Test
    EXPECT_NO_THROW({
        obj.doSomething();
    });
    
    // Verify
    EXPECT_TRUE(obj.wasSuccessful());
}
```

## Static Analysis Integration

For even better bug detection, integrate with static analysis tools:

```bash
# Clang Static Analyzer
scan-build make unit-tests

# Cppcheck
cppcheck --enable=all unit_tests/

# AddressSanitizer (compile-time flag)
make unit-tests CXXFLAGS="-fsanitize=address -g"
```

## Conclusion

This unit test suite provides:
- **Comprehensive coverage** of main.cpp functionality
- **Realistic bug detection** that finds actual issues
- **Performance monitoring** to catch regressions
- **Detailed reporting** for debugging and tracking
- **Multiple test categories** for organized testing
- **Easy integration** with build systems and CI/CD

The key insight is that **good unit tests should find bugs**, not just verify that perfect code works perfectly. The rigorous tests demonstrate this principle by intentionally including buggy implementations to show how proper testing should expose real issues.
