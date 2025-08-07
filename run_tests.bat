@echo off
REM ##############################################################################
REM Copyright 2015-2022 Xilinx, Inc.
REM Copyright 2022-2023 Advanced Micro Devices, Inc.
REM
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.
REM ##############################################################################
REM
REM Windows batch script to run Bootgen unit tests
REM
REM ##############################################################################

echo.
echo ========================================
echo    Bootgen Unit Test Runner
echo ========================================
echo.

REM Check if we're in the tests directory
if not exist "test_main.cpp" (
    echo Error: test_main.cpp not found. Please run this script from the tests directory.
    goto :error
)

REM Set up environment
set TEST_DIR=%cd%
set ROOT_DIR=%cd%\..

echo Setting up test environment...
echo Test directory: %TEST_DIR%
echo Root directory: %ROOT_DIR%

REM Check for required files
if not exist "%ROOT_DIR%\common\include\options.h" (
    echo Error: Required header files not found in %ROOT_DIR%\common\include\
    goto :error
)

echo.
echo Building tests...

REM For Windows, we'll use a simple compilation approach
REM Note: This assumes you have g++ or Visual Studio compiler in PATH

REM Try to compile with g++ (if available)
where g++ >nul 2>nul
if %ERRORLEVEL% == 0 (
    echo Using g++ compiler...
    g++ -std=c++11 -I..\common\include -I..\spartanup\include -I..\versal\include -I..\versal_2ve_2vm\include -I..\zynq\include -I..\zynqmp\include -I..\utils\include -I..\lms-hash-sigs -I..\win_include -I..\bisonflex -DTEST_MODE -o bootgen_tests.exe test_main.cpp
    if %ERRORLEVEL% == 0 (
        echo.
        echo Tests compiled successfully!
        echo.
        echo Running tests...
        bootgen_tests.exe
        goto :end
    ) else (
        echo Error: Compilation failed with g++
        goto :error
    )
)

REM Try to compile with cl (Visual Studio)
where cl >nul 2>nul
if %ERRORLEVEL% == 0 (
    echo Using Visual Studio compiler...
    cl /EHsc /I..\common\include /I..\spartanup\include /I..\versal\include /I..\versal_2ve_2vm\include /I..\zynq\include /I..\zynqmp\include /I..\utils\include /I..\lms-hash-sigs /I..\win_include /I..\bisonflex /DTEST_MODE test_main.cpp /Fe:bootgen_tests.exe
    if %ERRORLEVEL% == 0 (
        echo.
        echo Tests compiled successfully!
        echo.
        echo Running tests...
        bootgen_tests.exe
        goto :end
    ) else (
        echo Error: Compilation failed with Visual Studio compiler
        goto :error
    )
)

echo Error: No suitable C++ compiler found in PATH
echo Please install one of the following:
echo - MinGW/MSYS2 (for g++)
echo - Visual Studio Build Tools (for cl)
goto :error

:error
echo.
echo Test execution failed!
exit /b 1

:end
echo.
echo Tests completed!
exit /b 0
