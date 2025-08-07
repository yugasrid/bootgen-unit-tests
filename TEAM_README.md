# Bootgen Unit Testing Framework

## Quick Start for Team Members

### 1. Clone and Setup
```bash
# Clone the unit testing framework repository
git clone https://github.com/yugasrid/bootgen-unit-tests.git
cd bootgen-unit-tests

# Verify you have the right structure
ls -la
# Should see: Makefile, unit_tests/, README.md, etc.
```

### 2. Run All Tests
```bash
# Build and run all tests
make test-all

# OR run specific test categories
make test-basic        # Basic functionality tests
make test-args         # Argument parsing tests  
make test-exceptions   # Exception handling tests
make test-bif          # BIF file processing tests
make test-performance  # Performance and memory tests
make test-rigorous     # Rigorous bug detection tests
```

### 3. View Results
```bash
# Check terminal output for summary
# Check test_reports/ folder for detailed reports
cat test_reports/SUMMARY_REPORT.txt
```

## ⚠️ IMPORTANT: Current Framework Limitation

**This testing framework currently uses MOCK CLASSES instead of the actual Bootgen source code.**

### The Missing Link
- **Real Bootgen Source**: Located in `../common/src/main.cpp`, `../common/src/options.cpp`, etc.
- **Current Tests**: Use mock implementations in `unit_tests/mock_classes.h`
- **Missing**: Direct integration with actual Bootgen classes (`Options`, `BIF_File`, `BootGenApp`)

### To Test Real Bootgen Code
```bash
# Current structure tests mocks - NOT the real code
tests/unit_tests/mock_classes.h    # Mock Options, MockBIF_File
tests/unit_tests/test_*.cpp        # Tests using mocks

# Real Bootgen source code (NOT currently tested)
common/src/main.cpp                # Real BootGenApp class
common/src/options.cpp             # Real Options class  
common/include/options.h           # Real Options header
common/src/bifoptions.cpp          # Real BIF_File class
```

### What This Framework Provides

✅ **96+ Individual Tests** across 6 categories  
✅ **Real Bug Detection** - finds buffer overflows, null pointers, memory leaks  
✅ **Cross-Platform** - works on Windows (PowerShell) and Linux (Bash)  
✅ **Comprehensive Reports** - detailed statistics and individual test results  
✅ **Easy to Use** - single command runs everything  
✅ **Custom Test Framework** - no external dependencies required (no Google Test needed)
✅ **Organized Structure** - clean separation of test categories  

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

### ⚠️ To Test Real Bootgen Code (Advanced)
To test actual Bootgen classes instead of mocks:
1. **Include Real Headers**: Add `-I../common/include` to compiler flags
2. **Link Real Objects**: Compile and link with `../common/src/*.cpp` files
3. **Replace Mocks**: Use real `Options`, `BIF_File` classes instead of mocks
4. **Handle Dependencies**: Include OpenSSL, other dependencies as needed

Example for real integration:
```cpp
#include "../common/include/options.h"  // Real Options class
#include "../common/include/bifoptions.h"  // Real BIF_File class

// Test real Options class
void test_real_options_parsing() {
    Options realOptions;  // Real class, not mock
    const char* argv[] = {"bootgen", "-image", "test.bif"};
    realOptions.ParseArgs(3, argv);
    // Test real behavior
}
```

### Test Framework Features
- `EXPECT_TRUE(condition)` - verify condition is true
- `EXPECT_EQ(expected, actual)` - verify equality  
- `EXPECT_THROW(code, exception)` - verify exception thrown
- `EXPECT_NO_THROW(code)` - verify no exception
- `SUCCEED()` / `FAIL(message)` - explicit pass/fail

### Requirements
- C++11 compiler (g++) - required for compilation
- Make - for build system
- Bash (Linux) or PowerShell (Windows) - for test runners
- bc (for calculations) - typically pre-installed on most systems
- No external test libraries required (Google Test NOT needed)

## Next Steps for Real Integration

1. **Phase 1 (Current)**: Mock-based testing framework ✅
2. **Phase 2 (Needed)**: Integration with real Bootgen source classes
3. **Phase 3 (Future)**: Full integration testing with actual BIF files and boot images

## Questions?

Contact @yugasrid for questions about the testing framework or to suggest improvements.

Repository: https://github.com/yugasrid/bootgen-unit-tests
