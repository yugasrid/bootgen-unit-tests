#!/bin/bash
# Test runner script for Bootgen unit tests
# This script discovers and runs all unit tests in the unit_tests directory

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo "======================================="
echo "BOOTGEN UNIT TEST RUNNER"
echo "======================================="
echo ""

# Create reports directory if it doesn't exist
mkdir -p test_reports

# Initialize counters
total_tests=0
passed_tests=0
failed_tests=0
test_executables=()

# Find all test executables in build directory
echo "Discovering test executables..."
if [ -d "../build" ]; then
    for test_exe in ../build/test_*; do
        if [ -x "$test_exe" ] && [ -f "$test_exe" ]; then
            test_executables+=("$test_exe")
            echo "  Found: $(basename $test_exe)"
        fi
    done
else
    echo "Build directory not found. Please run 'make unit-tests' first."
    exit 1
fi

echo ""
echo "Found ${#test_executables[@]} test executable(s)"
echo ""

# Check if any tests were found
if [ ${#test_executables[@]} -eq 0 ]; then
    echo "No test executables found. Please run 'make unit-tests' first."
    exit 1
fi

# Run each test executable
for test_exe in "${test_executables[@]}"; do
    test_name=$(basename "$test_exe")
    echo "======================================="
    echo -e "${BLUE}Running: $test_name${NC}"
    echo "======================================="
    
    # Run the test and capture output
    if "$test_exe"; then
        echo -e "${GREEN}✓ $test_name PASSED${NC}"
        ((passed_tests++))
    else
        echo -e "${RED}✗ $test_name FAILED${NC}"
        ((failed_tests++))
    fi
    
    ((total_tests++))
    echo ""
done

# Move individual test reports to reports directory
echo "Moving test reports..."
mv *_report.txt test_reports/ 2>/dev/null || true

# Calculate total individual tests from all reports
total_individual_tests=0
total_passed_individual=0
total_failed_individual=0

for report in test_reports/*_report.txt; do
    if [ -f "$report" ]; then
        # Extract test counts from individual reports
        passed=$(grep "^Passed:" "$report" | awk '{print $2}' 2>/dev/null || echo "0")
        failed=$(grep "^Failed:" "$report" | awk '{print $2}' 2>/dev/null || echo "0")
        total=$(grep "^Total Tests:" "$report" | awk '{print $3}' 2>/dev/null || echo "0")
        
        # Add to totals (with safety checks)
        if [ "$passed" -eq "$passed" ] 2>/dev/null; then
            total_passed_individual=$((total_passed_individual + passed))
        fi
        if [ "$failed" -eq "$failed" ] 2>/dev/null; then
            total_failed_individual=$((total_failed_individual + failed))
        fi
        if [ "$total" -eq "$total" ] 2>/dev/null; then
            total_individual_tests=$((total_individual_tests + total))
        fi
    fi
done

# Generate summary report
summary_file="test_reports/SUMMARY_REPORT.txt"
echo "Generating summary report: $summary_file"

# Calculate success rate (avoid divide by zero)
if [ $total_tests -gt 0 ]; then
    success_rate=$(echo "scale=1; $passed_tests * 100 / $total_tests" | bc -l 2>/dev/null || echo "0.0")
else
    success_rate="0.0"
fi

# Calculate individual test success rate
if [ $total_individual_tests -gt 0 ]; then
    individual_success_rate=$(echo "scale=1; $total_passed_individual * 100 / $total_individual_tests" | bc -l 2>/dev/null || echo "0.0")
else
    individual_success_rate="0.0"
fi

cat > "$summary_file" << EOF
======================================
BOOTGEN UNIT TESTS - SUMMARY REPORT
======================================
Generated: $(date)

OVERALL RESULTS:
===============
Total Test Suites: $total_tests
Passed: $passed_tests
Failed: $failed_tests
Success Rate: ${success_rate}%

DETAILED TEST RESULTS:
=====================
Total Individual Tests: $total_individual_tests
Individual Tests Passed: $total_passed_individual
Individual Tests Failed: $total_failed_individual
Individual Success Rate: ${individual_success_rate}%

TEST SUITE DETAILS:
==================
EOF

# Add details for each test
for test_exe in "${test_executables[@]}"; do
    test_name=$(basename "$test_exe")
    if "$test_exe" > /dev/null 2>&1; then
        echo "$test_name: PASSED" >> "$summary_file"
    else
        echo "$test_name: FAILED" >> "$summary_file"
    fi
done

cat >> "$summary_file" << EOF

INDIVIDUAL REPORTS:
==================
EOF

# List individual report files
for report in test_reports/*_report.txt; do
    if [ -f "$report" ]; then
        echo "- $(basename $report)" >> "$summary_file"
    fi
done

# Display final summary
echo "======================================="
echo "FINAL SUMMARY"
echo "======================================="
echo -e "Total Test Suites: $total_tests"
echo -e "Passed: ${GREEN}$passed_tests${NC}"
echo -e "Failed: ${RED}$failed_tests${NC}"

# Display individual test counts
echo ""
echo "INDIVIDUAL TEST DETAILS:"
echo "========================"
echo -e "Total Individual Tests: ${BLUE}$total_individual_tests${NC}"
echo -e "Individual Tests Passed: ${GREEN}$total_passed_individual${NC}"
echo -e "Individual Tests Failed: ${RED}$total_failed_individual${NC}"
echo -e "Individual Success Rate: ${CYAN}${individual_success_rate}%${NC}"
echo ""

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}🎉 All test suites passed!${NC}"
    exit_code=0
else
    echo -e "${RED}❌ Some test suites failed.${NC}"
    echo -e "${YELLOW}Check individual reports in test_reports/ directory${NC}"
    exit_code=1
fi

echo ""
echo "Reports available in: test_reports/"
echo "Summary report: $summary_file"
echo "======================================="

exit $exit_code
