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

void test_BootGenApp_RunWithValidBifFile() {
    TestableBootGenApp app;
    const char* argv[] = {"bootgen", "-image", "test.bif", "-o", "output.bin"};
    int argc = 5;

    EXPECT_NO_THROW({
        app.Run(argc, argv);
    });
    
    EXPECT_TRUE(app.WasDisplayBannerCalled());
}

void test_BootGenApp_RunWithEmptyBifFile() {
    TestableBootGenApp app;
    const char* argv[] = {"bootgen"};
    int argc = 1;

    EXPECT_NO_THROW({
        app.Run(argc, argv);
    });
    
    EXPECT_TRUE(app.WasDisplayBannerCalled());
}

void test_BootGenApp_RunWithHelpArgument() {
    TestableBootGenApp app;
    const char* argv[] = {"bootgen", "-help"};
    int argc = 2;

    EXPECT_NO_THROW({
        app.Run(argc, argv);
    });
    
    EXPECT_TRUE(app.WasDisplayBannerCalled());
}

void test_BootGenApp_RunWithMultipleArguments() {
    TestableBootGenApp app;
    const char* argv[] = {"bootgen", "-arch", "zynq", "-image", "test.bif", "-o", "output.bin", "-verbose"};
    int argc = 8;

    EXPECT_NO_THROW({
        app.Run(argc, argv);
    });
    
    EXPECT_TRUE(app.WasDisplayBannerCalled());
}

void test_BootGenApp_WithMockOptions() {
    TestableBootGenApp app;
    MockOptions mockOpts;
    app.SetMockOptions(&mockOpts);
    
    const char* argv[] = {"bootgen", "-image", "mock.bif"};
    int argc = 3;

    EXPECT_NO_THROW({
        app.Run(argc, argv);
    });
    
    EXPECT_TRUE(app.WasDisplayBannerCalled());
}

int main() {
    std::cout << "Running Basic Functionality Tests..." << std::endl;
    std::cout << "====================================" << std::endl;

    RUN_TEST(test_BootGenApp_RunWithValidBifFile);
    RUN_TEST(test_BootGenApp_RunWithEmptyBifFile);
    RUN_TEST(test_BootGenApp_RunWithHelpArgument);
    RUN_TEST(test_BootGenApp_RunWithMultipleArguments);
    RUN_TEST(test_BootGenApp_WithMockOptions);

    print_test_summary();
    generate_test_report("basic_functionality_report.txt");
    
    return get_exit_code();
}
