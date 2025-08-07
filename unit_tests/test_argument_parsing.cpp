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

void test_ParseArgs_NoArguments() {
    MockOptions options;
    const char* argv[] = {"bootgen"};
    int argc = 1;
    
    EXPECT_NO_THROW({
        options.ParseArgs(argc, argv);
    });
    
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_EQ(1, options.arguments.size());
    EXPECT_STREQ("bootgen", options.arguments[0].c_str());
}

void test_ParseArgs_ImageArgument() {
    MockOptions options;
    const char* argv[] = {"bootgen", "-image", "test.bif"};
    int argc = 3;
    
    EXPECT_NO_THROW({
        options.ParseArgs(argc, argv);
    });
    
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_STREQ("test.bif", options.GetBifFilename().c_str());
    EXPECT_EQ(3, options.arguments.size());
}

void test_ParseArgs_OutputArgument() {
    MockOptions options;
    const char* argv[] = {"bootgen", "-image", "test.bif", "-o", "output.bin"};
    int argc = 5;
    
    EXPECT_NO_THROW({
        options.ParseArgs(argc, argv);
    });
    
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_STREQ("test.bif", options.GetBifFilename().c_str());
    EXPECT_STREQ("output.bin", options.GetOutputFilename().c_str());
}

void test_ParseArgs_ArchitectureArgument() {
    MockOptions options;
    const char* argv[] = {"bootgen", "-arch", "zynq", "-image", "test.bif"};
    int argc = 5;
    
    EXPECT_NO_THROW({
        options.ParseArgs(argc, argv);
    });
    
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_STREQ("zynq", options.GetArchitecture().c_str());
    EXPECT_STREQ("test.bif", options.GetBifFilename().c_str());
}

void test_ParseArgs_HelpArgument() {
    MockOptions options;
    const char* argv[] = {"bootgen", "-help"};
    int argc = 2;
    
    EXPECT_NO_THROW({
        options.ParseArgs(argc, argv);
    });
    
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_TRUE(options.IsHelpRequested());
}

void test_ParseArgs_VerboseArgument() {
    MockOptions options;
    const char* argv[] = {"bootgen", "-verbose", "-image", "test.bif"};
    int argc = 4;
    
    EXPECT_NO_THROW({
        options.ParseArgs(argc, argv);
    });
    
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_TRUE(options.IsVerboseMode());
    EXPECT_STREQ("test.bif", options.GetBifFilename().c_str());
}

void test_ParseArgs_AllArguments() {
    MockOptions options;
    const char* argv[] = {"bootgen", "-arch", "versal", "-image", "complex.bif", "-o", "final.bin", "-verbose"};
    int argc = 8;
    
    EXPECT_NO_THROW({
        options.ParseArgs(argc, argv);
    });
    
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_STREQ("versal", options.GetArchitecture().c_str());
    EXPECT_STREQ("complex.bif", options.GetBifFilename().c_str());
    EXPECT_STREQ("final.bin", options.GetOutputFilename().c_str());
    EXPECT_TRUE(options.IsVerboseMode());
    EXPECT_EQ(8, options.arguments.size());
}

void test_ParseArgs_Reset() {
    MockOptions options;
    const char* argv[] = {"bootgen", "-image", "test.bif", "-verbose"};
    int argc = 4;
    
    options.ParseArgs(argc, argv);
    EXPECT_TRUE(options.parseArgsCalled);
    EXPECT_TRUE(options.IsVerboseMode());
    
    options.Reset();
    EXPECT_FALSE(options.parseArgsCalled);
    EXPECT_FALSE(options.IsVerboseMode());
    EXPECT_TRUE(options.GetBifFilename().empty());
}

void test_ProcessMethods() {
    MockOptions options;
    
    EXPECT_FALSE(options.processVerifyKDFCalled);
    EXPECT_FALSE(options.processReadImageCalled);
    
    options.ProcessVerifyKDF();
    EXPECT_TRUE(options.processVerifyKDFCalled);
    
    options.ProcessReadImage();
    EXPECT_TRUE(options.processReadImageCalled);
}

int main() {
    std::cout << "Running Argument Parsing Tests..." << std::endl;
    std::cout << "=================================" << std::endl;

    RUN_TEST(test_ParseArgs_NoArguments);
    RUN_TEST(test_ParseArgs_ImageArgument);
    RUN_TEST(test_ParseArgs_OutputArgument);
    RUN_TEST(test_ParseArgs_ArchitectureArgument);
    RUN_TEST(test_ParseArgs_HelpArgument);
    RUN_TEST(test_ParseArgs_VerboseArgument);
    RUN_TEST(test_ParseArgs_AllArguments);
    RUN_TEST(test_ParseArgs_Reset);
    RUN_TEST(test_ProcessMethods);

    print_test_summary();
    generate_test_report("argument_parsing_report.txt");
    
    return get_exit_code();
}
