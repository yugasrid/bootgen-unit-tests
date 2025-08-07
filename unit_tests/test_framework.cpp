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

#include "test_framework.h"
#include <iomanip>

// Global test counters
int g_tests_passed = 0;
int g_tests_failed = 0;
std::vector<std::string> g_failed_tests;
std::vector<TestResult> g_test_results;

void generate_test_report(const std::string& filename) {
    std::ofstream report(filename);
    if (!report.is_open()) {
        std::cerr << "Failed to create test report file: " << filename << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    
    report << "======================================" << std::endl;
    report << "BOOTGEN UNIT TEST REPORT" << std::endl;
    report << "======================================" << std::endl;
    report << "Generated: " << std::ctime(&time);
    report << "Total Tests: " << (g_tests_passed + g_tests_failed) << std::endl;
    report << "Passed: " << g_tests_passed << std::endl;
    report << "Failed: " << g_tests_failed << std::endl;
    
    if (g_tests_failed + g_tests_passed > 0) {
        double success_rate = (double)g_tests_passed / (g_tests_passed + g_tests_failed) * 100.0;
        report << "Success Rate: " << std::fixed << std::setprecision(1) << success_rate << "%" << std::endl;
    }
    
    report << std::endl;

    // Detailed test results
    report << "DETAILED TEST RESULTS:" << std::endl;
    report << "======================================" << std::endl;
    
    for (const auto& result : g_test_results) {
        report << "Test: " << result.testName << std::endl;
        report << "  Status: " << (result.passed ? "PASSED" : "FAILED") << std::endl;
        report << "  Duration: " << result.duration.count() << "ms" << std::endl;
        if (!result.passed && !result.errorMessage.empty()) {
            report << "  Error: " << result.errorMessage << std::endl;
        }
        report << std::endl;
    }

    // Failed tests summary
    if (!g_failed_tests.empty()) {
        report << "FAILED TESTS SUMMARY:" << std::endl;
        report << "======================================" << std::endl;
        for (const auto& test : g_failed_tests) {
            report << "- " << test << std::endl;
        }
        report << std::endl;
    }

    // Performance summary
    if (!g_test_results.empty()) {
        report << "PERFORMANCE SUMMARY:" << std::endl;
        report << "======================================" << std::endl;
        
        auto total_duration = std::chrono::milliseconds(0);
        auto min_duration = g_test_results[0].duration;
        auto max_duration = g_test_results[0].duration;
        
        for (const auto& result : g_test_results) {
            total_duration += result.duration;
            if (result.duration < min_duration) min_duration = result.duration;
            if (result.duration > max_duration) max_duration = result.duration;
        }
        
        auto avg_duration = total_duration / g_test_results.size();
        
        report << "Total Execution Time: " << total_duration.count() << "ms" << std::endl;
        report << "Average Test Time: " << avg_duration.count() << "ms" << std::endl;
        report << "Fastest Test: " << min_duration.count() << "ms" << std::endl;
        report << "Slowest Test: " << max_duration.count() << "ms" << std::endl;
    }

    report.close();
    std::cout << "Test report generated: " << filename << std::endl;
}

void print_test_summary() {
    std::cout << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "TEST EXECUTION SUMMARY" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "Total Tests: " << (g_tests_passed + g_tests_failed) << std::endl;
    std::cout << "Passed: " << g_tests_passed << std::endl;
    std::cout << "Failed: " << g_tests_failed << std::endl;
    
    if (g_tests_failed + g_tests_passed > 0) {
        double success_rate = (double)g_tests_passed / (g_tests_passed + g_tests_failed) * 100.0;
        std::cout << "Success Rate: " << std::fixed << std::setprecision(1) << success_rate << "%" << std::endl;
    }
    
    if (!g_failed_tests.empty()) {
        std::cout << std::endl << "Failed Tests:" << std::endl;
        for (const auto& test : g_failed_tests) {
            std::cout << "  - " << test << std::endl;
        }
    }
    
    if (g_tests_failed == 0) {
        std::cout << std::endl << "🎉 All tests passed!" << std::endl;
    } else {
        std::cout << std::endl << "❌ Some tests failed. Check the report for details." << std::endl;
    }
    
    std::cout << "======================================" << std::endl;
}

int get_exit_code() {
    return (g_tests_failed == 0) ? 0 : 1;
}
