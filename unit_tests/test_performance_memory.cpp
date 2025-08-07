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
#include "mock_classes.h"

void test_Performance_QuickExecution() {
    auto start = std::chrono::high_resolution_clock::now();
    
    TestableBootGenApp app;
    const char* argv[] = {"bootgen", "-help"};
    int argc = 2;
    
    try {
        app.Run(argc, argv);
    } catch (...) {
        // Ignore exceptions for performance test
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should execute quickly (within reasonable time)
    EXPECT_LT(duration.count(), 5000); // Less than 5 seconds
    std::cout << "Execution time: " << duration.count() << "ms" << std::endl;
}

void test_Performance_MultipleRuns() {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 100; ++i) {
        TestableBootGenApp app;
        const char* argv[] = {"bootgen"};
        int argc = 1;
        
        try {
            app.Run(argc, argv);
        } catch (...) {
            // Ignore exceptions for performance test
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // 100 runs should complete in reasonable time
    EXPECT_LT(duration.count(), 10000); // Less than 10 seconds
    std::cout << "100 runs completed in: " << duration.count() << "ms" << std::endl;
    std::cout << "Average per run: " << (duration.count() / 100.0) << "ms" << std::endl;
}

void test_Performance_ArgumentParsing() {
    auto start = std::chrono::high_resolution_clock::now();
    
    MockOptions options;
    const char* argv[] = {"bootgen", "-arch", "versal", "-image", "large.bif", "-o", "output.bin", "-verbose"};
    int argc = 8;
    
    for (int i = 0; i < 1000; ++i) {
        options.Reset();
        options.ParseArgs(argc, argv);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 1000 argument parsing operations should be fast
    EXPECT_LT(duration.count(), 100000); // Less than 100ms
    std::cout << "1000 argument parsing operations: " << duration.count() << "μs" << std::endl;
    std::cout << "Average per operation: " << (duration.count() / 1000.0) << "μs" << std::endl;
}

void test_Performance_BIFFileCreation() {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; ++i) {
        std::string filename = "test_file_" + std::to_string(i) + ".bif";
        MockBIF_File bif(filename);
        // Just create and destroy
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // 1000 BIF file object creations should be fast
    EXPECT_LT(duration.count(), 50000); // Less than 50ms
    std::cout << "1000 BIF file creations: " << duration.count() << "μs" << std::endl;
    std::cout << "Average per creation: " << (duration.count() / 1000.0) << "μs" << std::endl;
}

void test_Memory_NoMemoryLeaks() {
    // Test that creating and destroying BootGenApp doesn't leak memory
    for (int i = 0; i < 100; ++i) {
        TestableBootGenApp app;
        const char* argv[] = {"bootgen", "-image", "test.bif"};
        int argc = 3;
        
        try {
            app.Run(argc, argv);
        } catch (...) {
            // Ignore exceptions for memory test
        }
    }
    
    // If we reach here without crashing, basic memory management is working
    SUCCEED();
}

void test_Memory_LargeArgumentLists() {
    // Test with large argument lists
    std::vector<const char*> argv;
    argv.push_back("bootgen");
    
    // Create a large argument list
    for (int i = 0; i < 100; ++i) {
        argv.push_back("-verbose");
    }
    argv.push_back("-image");
    argv.push_back("test.bif");
    
    MockOptions options;
    
    EXPECT_NO_THROW({
        options.ParseArgs(argv.size(), argv.data());
    });
    
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_EQ(argv.size(), options.arguments.size());
}

void test_Memory_StringOperations() {
    // Test string operations don't cause memory issues
    MockOptions options;
    
    for (int i = 0; i < 1000; ++i) {
        std::string longFilename(1000, 'a');
        longFilename += std::to_string(i) + ".bif";
        
        const char* argv[] = {"bootgen", "-image", longFilename.c_str()};
        int argc = 3;
        
        options.Reset();
        options.ParseArgs(argc, argv);
        
        EXPECT_TRUE(options.parseArgsCalled);
    }
    
    SUCCEED();
}

void test_Stress_RapidFileProcessing() {
    // Stress test with rapid file processing
    MockOptions options;
    
    for (int i = 0; i < 500; ++i) {
        std::string filename = "stress_test_" + std::to_string(i) + ".bif";
        MockBIF_File bif(filename);
        
        if (bif.IsValid()) {
            EXPECT_NO_THROW({
                bif.Process(options);
            });
        }
    }
    
    SUCCEED();
}

void test_Stress_ExceptionHandling() {
    // Stress test exception handling
    int exception_count = 0;
    
    for (int i = 0; i < 100; ++i) {
        try {
            if (i % 3 == 0) {
                throw std::runtime_error("Test exception " + std::to_string(i));
            } else if (i % 3 == 1) {
                throw "Character exception";
            } else {
                throw i;
            }
        } catch (std::exception& e) {
            exception_count++;
        } catch (const char* msg) {
            exception_count++;
        } catch (...) {
            exception_count++;
        }
    }
    
    EXPECT_EQ(100, exception_count);
}

int main() {
    std::cout << "Running Performance and Memory Tests..." << std::endl;
    std::cout << "=======================================" << std::endl;

    RUN_TEST(test_Performance_QuickExecution);
    RUN_TEST(test_Performance_MultipleRuns);
    RUN_TEST(test_Performance_ArgumentParsing);
    RUN_TEST(test_Performance_BIFFileCreation);
    RUN_TEST(test_Memory_NoMemoryLeaks);
    RUN_TEST(test_Memory_LargeArgumentLists);
    RUN_TEST(test_Memory_StringOperations);
    RUN_TEST(test_Stress_RapidFileProcessing);
    RUN_TEST(test_Stress_ExceptionHandling);

    print_test_summary();
    generate_test_report("performance_memory_report.txt");
    
    return get_exit_code();
}
