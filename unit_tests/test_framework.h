/******************************************************************************
* Copyright 2015-2022 Xilinx, Inc.
* Copyright 2022-2023 Advanced Micro Devices, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
* http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
******************************************************************************/

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <chrono>
#include <fstream>

// Global test counters
extern int g_tests_passed;
extern int g_tests_failed;
extern std::vector<std::string> g_failed_tests;

// Test result tracking
struct TestResult {
    std::string testName;
    bool passed;
    std::string errorMessage;
    std::chrono::milliseconds duration;
};

extern std::vector<TestResult> g_test_results;

// Simple test framework macros
#define EXPECT_NO_THROW(statement) \
    do { \
        try { \
            statement; \
            std::cout << "[PASS] No exception thrown" << std::endl; \
            g_tests_passed++; \
        } catch (const std::exception& e) { \
            std::cout << "[FAIL] Unexpected exception thrown: " << e.what() << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } catch (...) { \
            std::cout << "[FAIL] Unexpected unknown exception thrown" << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_THROW(statement, exception_type) \
    do { \
        try { \
            statement; \
            std::cout << "[FAIL] Expected exception not thrown" << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } catch (const exception_type&) { \
            std::cout << "[PASS] Expected exception caught" << std::endl; \
            g_tests_passed++; \
        } catch (const std::exception& e) { \
            std::cout << "[FAIL] Wrong exception type thrown: " << e.what() << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } catch (...) { \
            std::cout << "[FAIL] Wrong exception type thrown (unknown)" << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_EQ(expected, actual) \
    do { \
        if ((expected) == (actual)) { \
            std::cout << "[PASS] Values equal: " << (expected) << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] Expected: " << (expected) << ", Actual: " << (actual) << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_NE(val1, val2) \
    do { \
        if ((val1) != (val2)) { \
            std::cout << "[PASS] Values not equal: " << (val1) << " != " << (val2) << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] Values should not be equal: " << (val1) << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_TRUE(condition) \
    do { \
        if (condition) { \
            std::cout << "[PASS] Condition true" << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] Condition false" << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_FALSE(condition) \
    do { \
        if (!(condition)) { \
            std::cout << "[PASS] Condition false" << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] Condition should be false" << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_LT(val1, val2) \
    do { \
        if ((val1) < (val2)) { \
            std::cout << "[PASS] " << (val1) << " < " << (val2) << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] " << (val1) << " not < " << (val2) << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_GT(val1, val2) \
    do { \
        if ((val1) > (val2)) { \
            std::cout << "[PASS] " << (val1) << " > " << (val2) << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] " << (val1) << " not > " << (val2) << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_LE(val1, val2) \
    do { \
        if ((val1) <= (val2)) { \
            std::cout << "[PASS] " << (val1) << " <= " << (val2) << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] " << (val1) << " not <= " << (val2) << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_GE(val1, val2) \
    do { \
        if ((val1) >= (val2)) { \
            std::cout << "[PASS] " << (val1) << " >= " << (val2) << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] " << (val1) << " not >= " << (val2) << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_STREQ(str1, str2) \
    do { \
        if (std::string(str1) == std::string(str2)) { \
            std::cout << "[PASS] Strings equal" << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] Expected: '" << (str1) << "', Actual: '" << (str2) << "'" << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define EXPECT_STRNE(str1, str2) \
    do { \
        if (std::string(str1) != std::string(str2)) { \
            std::cout << "[PASS] Strings not equal" << std::endl; \
            g_tests_passed++; \
        } else { \
            std::cout << "[FAIL] Strings should not be equal: '" << (str1) << "'" << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(__func__); \
        } \
    } while(0)

#define SUCCEED() \
    do { \
        std::cout << "[PASS] Test succeeded" << std::endl; \
        g_tests_passed++; \
    } while(0)

#define FAIL(message) \
    do { \
        std::cout << "[FAIL] " << (message) << std::endl; \
        g_tests_failed++; \
        g_failed_tests.push_back(__func__); \
    } while(0)

// Test registration macros
#define RUN_TEST(test_func) \
    do { \
        std::cout << "\n=== Running: " << #test_func << " ===" << std::endl; \
        auto start_time = std::chrono::high_resolution_clock::now(); \
        int initial_failed = g_tests_failed; \
        try { \
            test_func(); \
        } catch (const std::exception& e) { \
            std::cout << "[EXCEPTION] " << e.what() << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(#test_func); \
        } catch (...) { \
            std::cout << "[UNKNOWN EXCEPTION]" << std::endl; \
            g_tests_failed++; \
            g_failed_tests.push_back(#test_func); \
        } \
        auto end_time = std::chrono::high_resolution_clock::now(); \
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time); \
        TestResult result; \
        result.testName = #test_func; \
        result.passed = (g_tests_failed == initial_failed); \
        result.duration = duration; \
        if (!result.passed && g_tests_failed > initial_failed) { \
            result.errorMessage = "Test failed with assertions"; \
        } \
        g_test_results.push_back(result); \
        std::cout << "Test completed in " << duration.count() << "ms" << std::endl; \
    } while(0)

// Test report functions
void generate_test_report(const std::string& filename = "test_report.txt");
void print_test_summary();
int get_exit_code();

// Test base class
class TestBase {
public:
    virtual ~TestBase() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}
};

#endif // TEST_FRAMEWORK_H
