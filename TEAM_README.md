# Bootgen Unit Testing Framework

## Quick Start for Team Members

### 1. Clone and Setup
```bash
# If using separate repository
git clone https://github.com/[YOUR_USERNAME]/bootgen-unit-tests.git
cd bootgen-unit-tests

# OR if part of your bootgen fork
git clone https://github.com/[YOUR_USERNAME]/bootgen.git
cd bootgen/tests
```

### 2. Run All Tests
```bash
# Build and run all tests
make test-all
```

### 3. View Results
```bash
# Check terminal output for summary
# Check test_reports/ folder for detailed reports
cat test_reports/SUMMARY_REPORT.txt
```

## What This Framework Provides

✅ **96+ Individual Tests** across 6 categories  
✅ **Real Bug Detection** - finds buffer overflows, null pointers, memory leaks  
✅ **Cross-Platform** - works on Windows and Linux  
✅ **Comprehensive Reports** - detailed statistics and individual test results  
✅ **Easy to Use** - single command runs everything  

## Test Categories

| Command | Category | Tests | Purpose |
|---------|----------|-------|---------|
| `make test-basic` | Basic Functionality | ~10 | Core app behavior |
| `make test-args` | Argument Parsing | ~39 | Command-line handling |
| `make test-exceptions` | Exception Handling | ~8 | Error conditions |
| `make test-bif` | BIF File Processing | ~12 | File operations |
| `make test-performance` | Performance & Memory | ~10 | Resource validation |
| `make test-rigorous` | Bug Detection | ~17 | Real bug finding |

## Example Output

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

## For Developers

### Adding New Tests
1. Create new test file in `unit_tests/test_new_feature.cpp`
2. Use framework: `#include "test_framework.h"`
3. Add `RUN_TEST(test_function_name)` calls
4. Update Makefile with new target

### Test Framework Features
- `EXPECT_TRUE(condition)` - verify condition is true
- `EXPECT_EQ(expected, actual)` - verify equality  
- `EXPECT_THROW(code, exception)` - verify exception thrown
- `EXPECT_NO_THROW(code)` - verify no exception
- `SUCCEED()` / `FAIL(message)` - explicit pass/fail

### Requirements
- C++11 compiler (g++)
- Make 
- Bash (Linux) or PowerShell (Windows)
- bc (for calculations)

## Benefits for Our Team

🔍 **Quality Assurance** - Catches bugs before they reach production  
📊 **Regression Testing** - Ensures changes don't break existing functionality  
📝 **Documentation** - Tests serve as executable examples  
🚀 **Confidence** - Know your changes work correctly  

## Questions?

Contact [Your Name] for questions about the testing framework.

Repository: https://github.com/[YOUR_USERNAME]/bootgen-unit-tests
