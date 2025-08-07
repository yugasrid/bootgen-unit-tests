# Bootgen Unit Testing Framework

## Overview

Comprehensive unit testing framework for Xilinx Bootgen with **96+ individual tests** across **6 organized categories**. Designed to detect real bugs including buffer overflows, null pointers, memory leaks, and other critical issues.

## ✨ Key Features

- 🔧 **Self-contained** - No external dependencies (Google Test, etc.)
- 🐛 **Real bug detection** - Finds buffer overflows, null pointers, memory leaks  
- 🌐 **Cross-platform** - Windows PowerShell + Linux Bash support
- 📊 **Comprehensive reporting** - Individual and summary reports with statistics
- 📁 **Organized structure** - 6 test categories for maintainability
- ⚡ **Easy to use** - Single command runs everything

## Quick Start

```bash
# Clone repository
git clone https://github.com/yugasrid/bootgen-unit-tests.git
cd bootgen-unit-tests

# Run all tests
make test-all
```

## Test Categories

| Command | Category | Tests | Purpose |
|---------|----------|-------|---------|
| `make test-basic` | Basic Functionality | ~10 | Core app behavior |
| `make test-args` | Argument Parsing | ~39 | Command-line handling |
| `make test-exceptions` | Exception Handling | ~8 | Error conditions |
| `make test-bif` | BIF File Processing | ~12 | File operations |
| `make test-performance` | Performance & Memory | ~10 | Resource validation |
| `make test-rigorous` | Bug Detection | ~17 | Real bug finding |

## Example Results

```
=======================================
FINAL SUMMARY
=======================================
Total Test Suites: 6
Passed: 5
Failed: 1

INDIVIDUAL TEST DETAILS:
========================
Total Individual Tests: 96
Individual Tests Passed: 95
Individual Tests Failed: 1
Individual Success Rate: 98.9%

✅ Most test suites passed!
❌ Some test suites failed.
Check individual reports in test_reports/ directory
```

## Directory Structure

```
├── Makefile                     # Build system for all tests
├── README.md                    # This documentation
├── TEAM_README.md               # Team usage guide
├── unit_tests/                  # Organized test implementation
│   ├── test_framework.h         # Custom test framework header
│   ├── test_framework.cpp       # Framework implementation
│   ├── mock_classes.h           # Mock classes with intentional bugs
│   ├── test_basic_functionality.cpp      # Core functionality tests
│   ├── test_argument_parsing.cpp         # Command-line argument tests
│   ├── test_exception_handling.cpp       # Exception handling tests
│   ├── test_bif_file_processing.cpp      # BIF file processing tests
│   ├── test_performance_memory.cpp       # Performance and memory tests
│   ├── test_rigorous_bug_detection.cpp   # Rigorous bug detection tests
│   ├── run_tests.sh             # Bash test runner
│   ├── run_tests.ps1            # PowerShell test runner
│   └── test_reports/            # Generated test reports
├── build/                       # Built test executables
├── test_reports/                # Summary reports
├── test_main.cpp               # Legacy test (backward compatibility)
└── run_tests.ps1               # Legacy PowerShell runner
```

## Usage Examples

### Run All Tests (Recommended)
```bash
make test-all
```

### Run Specific Test Categories
```bash
make test-basic           # Basic functionality tests
make test-args            # Argument parsing tests  
make test-exceptions      # Exception handling tests
make test-bif             # BIF file processing tests
make test-performance     # Performance and memory tests
make test-rigorous        # Rigorous bug detection tests
```

### View Detailed Reports
```bash
# View summary report
cat unit_tests/test_reports/SUMMARY_REPORT.txt

# View individual test reports
ls unit_tests/test_reports/
cat unit_tests/test_reports/basic_functionality_report.txt
```

### Build Individual Tests
```bash
make unit-tests           # Build all test executables
make test_basic_functionality    # Build specific test
```

## Bug Detection Capabilities

The framework successfully detects:

### Buffer Overflows
- Fixed-size buffer overflow detection
- Long filename handling validation
- Output parameter bounds checking

### Memory Issues  
- Null pointer dereferences
- Memory leaks detection
- Double-delete scenarios
- Resource exhaustion testing

### Input Validation
- Malicious input handling
- Path traversal attempts  
- Boundary condition testing
- Special character processing

### Example Detected Issues
```cpp
// Buffer overflow in RealisticOptions
char outputFileName[256];  // Fixed buffer
strcpy(outputFileName, argv[i + 1]);  // No bounds checking

// Null pointer issue in RealisticBIF_File  
const char* bifName = options.GetBifFilename();
if (strlen(bifName) > 10000) {  // No null check first!
```

## Requirements

- **C++11 compiler** (g++)
- **Make** build system
- **Bash** (Linux/macOS) or **PowerShell** (Windows)
- **bc** calculator (for bash scripts)

## For Developers

### Adding New Tests

1. **Create new test file** in `unit_tests/test_new_feature.cpp`
2. **Include framework**: `#include "test_framework.h"`
3. **Write test functions**:
   ```cpp
   void test_NewFeature() {
       EXPECT_TRUE(some_condition);
       EXPECT_EQ(expected, actual);
   }
   ```
4. **Add to main function**:
   ```cpp
   int main() {
       RUN_TEST(test_NewFeature);
       print_test_summary();
       generate_test_report("new_feature_report.txt");
       return get_exit_code();
   }
   ```
5. **Update Makefile** with new target

### Test Framework Macros

```cpp
EXPECT_TRUE(condition)         // Verify condition is true
EXPECT_FALSE(condition)        // Verify condition is false  
EXPECT_EQ(expected, actual)    // Verify equality
EXPECT_NE(expected, actual)    // Verify inequality
EXPECT_THROW(code, exception)  // Verify exception is thrown
EXPECT_NO_THROW(code)          // Verify no exception is thrown
SUCCEED()                      // Mark test as passed
FAIL(message)                  // Mark test as failed with message
```

### Best Practices

- **Test edge cases**: Empty inputs, null pointers, boundary values
- **Use descriptive names**: `test_ArgumentParsing_EmptyInput`
- **One concept per test**: Keep tests focused and specific
- **Document expected failures**: Use comments for intentional bugs
- **Verify both paths**: Test success and failure scenarios

## Cross-Platform Support

### Linux/macOS
```bash
cd unit_tests
bash run_tests.sh
```

### Windows
```powershell
cd unit_tests
./run_tests.ps1
```

### Legacy Windows (PowerShell)
```powershell
./run_tests.ps1
```

## Troubleshooting

### Compilation Issues
```bash
# Clean and rebuild
make clean
make unit-tests

# Check compiler
g++ --version
```

### Missing Reports
```bash
# Verify test executables exist
ls build/test_*

# Run tests manually
./build/test_basic_functionality
```

### Performance Issues
```bash
# Run with timing
time make test-all

# Run single test for debugging
make test-basic
```

## Integration with CI/CD

```bash
#!/bin/bash
# CI/CD integration example
cd tests
make clean
make test-all
exit_code=$?

if [ $exit_code -eq 0 ]; then
    echo "✅ All tests passed successfully"
else
    echo "❌ Tests failed, check reports in test_reports/"
    cat unit_tests/test_reports/SUMMARY_REPORT.txt
fi

exit $exit_code
```

## Performance Metrics

- **Full test suite**: ~1-2 seconds
- **Individual test suite**: ~0.1-0.3 seconds  
- **Individual test**: ~0-1ms
- **Memory usage**: ~10-50MB during execution
- **Report generation**: ~500KB total

## Contributing

When adding new tests:
1. Focus on edge cases and error conditions
2. Use descriptive test names  
3. Document expected failures
4. Keep tests focused and specific
5. Update documentation for new categories

## Author

**Yugasri Dhaded**  
GitHub: [@yugasrid](https://github.com/yugasrid)

## License

Licensed under the Apache License, Version 2.0. See the main Bootgen project for full license details.

---

*Last updated: August 7, 2025*  
*Framework version: 1.0*  
*Total tests: 96+ individual tests across 6 categories*
