##############################################################################
#
# Copyright 2015-2022 Xilinx, Inc.
# Copyright 2022-2023 Advanced Micro Devices, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################################
#
# Makefile for Bootgen Unit Tests
#
##############################################################################

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wno-reorder -Wno-deprecated-declarations -g

# Directories
UNIT_TEST_DIR = unit_tests
BUILD_DIR = build
COMMON_INC = ../common/include

# Include directories
INCLUDES = -I$(UNIT_TEST_DIR) \
          -I$(COMMON_INC) \
          -I../spartanup/include \
          -I../versal/include \
          -I../versal_2ve_2vm/include \
          -I../zynq/include \
          -I../zynqmp/include \
          -I../utils/include \
          -I../lms-hash-sigs \
          -I../win_include \
          -I../bisonflex

# Libraries
LIBS = -lpthread

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Test framework
$(BUILD_DIR)/test_framework.o: $(UNIT_TEST_DIR)/test_framework.cpp $(UNIT_TEST_DIR)/test_framework.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Individual unit test executables
$(BUILD_DIR)/test_basic_functionality: $(UNIT_TEST_DIR)/test_basic_functionality.cpp $(BUILD_DIR)/test_framework.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

$(BUILD_DIR)/test_argument_parsing: $(UNIT_TEST_DIR)/test_argument_parsing.cpp $(BUILD_DIR)/test_framework.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

$(BUILD_DIR)/test_exception_handling: $(UNIT_TEST_DIR)/test_exception_handling.cpp $(BUILD_DIR)/test_framework.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

$(BUILD_DIR)/test_bif_file_processing: $(UNIT_TEST_DIR)/test_bif_file_processing.cpp $(BUILD_DIR)/test_framework.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

$(BUILD_DIR)/test_performance_memory: $(UNIT_TEST_DIR)/test_performance_memory.cpp $(BUILD_DIR)/test_framework.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

$(BUILD_DIR)/test_rigorous_bug_detection: $(UNIT_TEST_DIR)/test_rigorous_bug_detection.cpp $(BUILD_DIR)/test_framework.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

# Legacy test (for backward compatibility)
$(BUILD_DIR)/bootgen_tests: test_main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

# Build all unit tests
unit-tests: $(BUILD_DIR)/test_basic_functionality \
           $(BUILD_DIR)/test_argument_parsing \
           $(BUILD_DIR)/test_exception_handling \
           $(BUILD_DIR)/test_bif_file_processing \
           $(BUILD_DIR)/test_performance_memory \
           $(BUILD_DIR)/test_rigorous_bug_detection

# Build legacy test
legacy-test: $(BUILD_DIR)/bootgen_tests

# Run all unit tests with comprehensive reporting
test-all: unit-tests
	@echo "======================================="
	@echo "Running All Unit Tests"
	@echo "======================================="
	cd $(UNIT_TEST_DIR) && bash run_tests.sh

# Run individual test categories
test-basic: $(BUILD_DIR)/test_basic_functionality
	@echo "Running Basic Functionality Tests..."
	./$(BUILD_DIR)/test_basic_functionality

test-args: $(BUILD_DIR)/test_argument_parsing
	@echo "Running Argument Parsing Tests..."
	./$(BUILD_DIR)/test_argument_parsing

test-exceptions: $(BUILD_DIR)/test_exception_handling
	@echo "Running Exception Handling Tests..."
	./$(BUILD_DIR)/test_exception_handling

test-bif: $(BUILD_DIR)/test_bif_file_processing
	@echo "Running BIF File Processing Tests..."
	./$(BUILD_DIR)/test_bif_file_processing

test-performance: $(BUILD_DIR)/test_performance_memory
	@echo "Running Performance and Memory Tests..."
	./$(BUILD_DIR)/test_performance_memory

test-rigorous: $(BUILD_DIR)/test_rigorous_bug_detection
	@echo "Running Rigorous Bug Detection Tests..."
	./$(BUILD_DIR)/test_rigorous_bug_detection

# Run legacy test (backward compatibility)
test-legacy: $(BUILD_DIR)/bootgen_tests
	@echo "Running Legacy Tests..."
	./$(BUILD_DIR)/bootgen_tests

# Clean
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(UNIT_TEST_DIR)/test_reports/*

# Help
help:
	@echo "Available targets:"
	@echo "  unit-tests     - Build all organized unit test executables"
	@echo "  test-all       - Build and run all unit tests with comprehensive reporting"
	@echo "  test-basic     - Run basic functionality tests"
	@echo "  test-args      - Run argument parsing tests"
	@echo "  test-exceptions- Run exception handling tests"
	@echo "  test-bif       - Run BIF file processing tests"
	@echo "  test-performance - Run performance and memory tests"
	@echo "  test-rigorous  - Run rigorous bug detection tests"
	@echo "  legacy-test    - Build legacy test executable (test_main.cpp)"
	@echo "  test-legacy    - Run legacy tests"
	@echo "  clean          - Remove all build artifacts and reports"
	@echo "  help           - Show this help message"
	@echo ""
	@echo "Organized Unit Tests Structure:"
	@echo "  unit_tests/                 - All organized test files"
	@echo "  unit_tests/test_reports/    - Generated test reports"
	@echo "  build/                      - Built test executables"
	@echo ""
	@echo "Note: Unit tests are self-contained with custom test framework"
	@echo "Rigorous tests are designed to expose real bugs and may fail intentionally"

.PHONY: unit-tests legacy-test test-all test-basic test-args test-exceptions test-bif test-performance test-rigorous test-legacy clean help
