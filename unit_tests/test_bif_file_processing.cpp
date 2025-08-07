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

void test_BIF_File_ValidFilename() {
    MockBIF_File bif("valid.bif");
    EXPECT_TRUE(bif.IsValid());
    EXPECT_STREQ("valid.bif", bif.filename.c_str());
    EXPECT_TRUE(bif.GetErrorMessage().empty());
}

void test_BIF_File_EmptyFilename() {
    MockBIF_File bif("");
    EXPECT_FALSE(bif.IsValid());
    EXPECT_STREQ("Empty filename provided", bif.GetErrorMessage().c_str());
}

void test_BIF_File_LongFilename() {
    std::string longName(1001, 'a');
    longName += ".bif";
    
    MockBIF_File bif(longName);
    EXPECT_FALSE(bif.IsValid());
    EXPECT_STREQ("Filename too long", bif.GetErrorMessage().c_str());
}

void test_BIF_File_InvalidPattern() {
    MockBIF_File bif("invalid_pattern.bif");
    EXPECT_FALSE(bif.IsValid());
    EXPECT_STREQ("Invalid filename pattern", bif.GetErrorMessage().c_str());
}

void test_BIF_File_ProcessValid() {
    MockBIF_File bif("test.bif");
    MockOptions options;
    
    EXPECT_NO_THROW({
        bif.Process(options);
    });
    
    EXPECT_TRUE(bif.processCalled);
}

void test_BIF_File_ProcessInvalid() {
    MockBIF_File bif("");  // Empty filename
    MockOptions options;
    
    EXPECT_THROW({
        bif.Process(options);
    }, std::runtime_error);
}

void test_BIF_File_ProcessWithThrowPattern() {
    MockBIF_File bif("throw_error.bif");
    MockOptions options;
    
    EXPECT_THROW({
        bif.Process(options);
    }, std::runtime_error);
}

void test_BIF_File_MultipleFiles() {
    std::vector<std::string> filenames = {
        "file1.bif",
        "file2.bif", 
        "file3.bif"
    };
    
    MockOptions options;
    
    for (const auto& filename : filenames) {
        MockBIF_File bif(filename);
        EXPECT_TRUE(bif.IsValid());
        
        EXPECT_NO_THROW({
            bif.Process(options);
        });
        
        EXPECT_TRUE(bif.processCalled);
    }
}

void test_BIF_File_EdgeCases() {
    // Test various edge cases
    std::vector<std::pair<std::string, bool>> testCases = {
        {"normal.bif", true},
        {"", false},                    // Empty
        {"a", true},                    // Single character
        {"file.txt", true},             // Different extension
        {"no_extension", true},         // No extension
        {"invalid_test.bif", false},    // Contains "invalid"
        {"../parent.bif", true},        // Path traversal (allowed in mock)
        {"file with spaces.bif", true}, // Spaces
        {"file\twith\ttabs.bif", true}, // Tabs
        {"üñíçøðé.bif", true}          // Unicode characters
    };
    
    for (const auto& testCase : testCases) {
        if (testCase.first.length() <= 1000) {  // Skip long names for edge case testing
            MockBIF_File bif(testCase.first);
            if (testCase.second) {
                EXPECT_TRUE(bif.IsValid());
            } else {
                EXPECT_FALSE(bif.IsValid());
            }
        }
    }
}

void test_BIF_File_ProcessingState() {
    MockBIF_File bif("state_test.bif");
    MockOptions options;
    
    // Initial state
    EXPECT_FALSE(bif.processCalled);
    EXPECT_TRUE(bif.IsValid());
    
    // After processing
    bif.Process(options);
    EXPECT_TRUE(bif.processCalled);
}

int main() {
    std::cout << "Running BIF File Processing Tests..." << std::endl;
    std::cout << "====================================" << std::endl;

    RUN_TEST(test_BIF_File_ValidFilename);
    RUN_TEST(test_BIF_File_EmptyFilename);
    RUN_TEST(test_BIF_File_LongFilename);
    RUN_TEST(test_BIF_File_InvalidPattern);
    RUN_TEST(test_BIF_File_ProcessValid);
    RUN_TEST(test_BIF_File_ProcessInvalid);
    RUN_TEST(test_BIF_File_ProcessWithThrowPattern);
    RUN_TEST(test_BIF_File_MultipleFiles);
    RUN_TEST(test_BIF_File_EdgeCases);
    RUN_TEST(test_BIF_File_ProcessingState);

    print_test_summary();
    generate_test_report("bif_file_processing_report.txt");
    
    return get_exit_code();
}
