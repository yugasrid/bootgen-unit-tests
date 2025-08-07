# ##############################################################################
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
# ##############################################################################
#
# PowerShell script to run Bootgen unit tests
#
# ##############################################################################

param(
    [switch]$Verbose,
    [switch]$Help,
    [string]$Compiler = "auto"
)

function Show-Help {
    Write-Host ""
    Write-Host "Bootgen Unit Test Runner (PowerShell)"
    Write-Host "====================================="
    Write-Host ""
    Write-Host "Usage: .\run_tests.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Verbose     Show detailed compilation output"
    Write-Host "  -Help        Show this help message"
    Write-Host "  -Compiler    Specify compiler: 'gcc', 'cl', or 'auto' (default)"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\run_tests.ps1                    # Auto-detect compiler and run tests"
    Write-Host "  .\run_tests.ps1 -Verbose           # Show detailed output"
    Write-Host "  .\run_tests.ps1 -Compiler gcc      # Force use of g++"
    Write-Host ""
}

function Test-Command {
    param([string]$Command)
    
    try {
        Get-Command $Command -ErrorAction Stop | Out-Null
        return $true
    } catch {
        return $false
    }
}

function Build-WithGCC {
    param([bool]$Verbose)
    
    Write-Host "Using g++ compiler..." -ForegroundColor Green
    
    $includes = @(
        "-I..\common\include",
        "-I..\spartanup\include", 
        "-I..\versal\include",
        "-I..\versal_2ve_2vm\include",
        "-I..\zynq\include",
        "-I..\zynqmp\include",
        "-I..\utils\include",
        "-I..\lms-hash-sigs",
        "-I..\win_include",
        "-I..\bisonflex"
    )
    
    $arguments = @(
        "-std=c++11",
        "-Wall",
        "-O2"
    ) + $includes + @(
        "-DTEST_MODE",
        "-o", "bootgen_tests.exe",
        "test_main_simple.cpp"
    )
    
    if ($Verbose) {
        Write-Host "Command: g++ $($arguments -join ' ')" -ForegroundColor Yellow
    }
    
    $process = Start-Process -FilePath "g++" -ArgumentList $arguments -Wait -PassThru -NoNewWindow
    return $process.ExitCode -eq 0
}

function Build-WithCL {
    param([bool]$Verbose)
    
    Write-Host "Using Visual Studio compiler..." -ForegroundColor Green
    
    $includes = @(
        "/I..\common\include",
        "/I..\spartanup\include",
        "/I..\versal\include", 
        "/I..\versal_2ve_2vm\include",
        "/I..\zynq\include",
        "/I..\zynqmp\include",
        "/I..\utils\include",
        "/I..\lms-hash-sigs",
        "/I..\win_include",
        "/I..\bisonflex"
    )
    
    $arguments = @(
        "/EHsc",
        "/W3"
    ) + $includes + @(
        "/DTEST_MODE",
        "test_main_simple.cpp",
        "/Fe:bootgen_tests.exe"
    )
    
    if ($Verbose) {
        Write-Host "Command: cl $($arguments -join ' ')" -ForegroundColor Yellow
    }
    
    $process = Start-Process -FilePath "cl" -ArgumentList $arguments -Wait -PassThru -NoNewWindow
    return $process.ExitCode -eq 0
}

function Main {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "    Bootgen Unit Test Runner" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    
    if ($Help) {
        Show-Help
        return
    }
    
    # Check if we're in the right directory
    if (-not (Test-Path "test_main_simple.cpp")) {
        Write-Host "Error: test_main_simple.cpp not found." -ForegroundColor Red
        Write-Host "Please run this script from the tests directory." -ForegroundColor Red
        exit 1
    }
    
    # Check for required files
    if (-not (Test-Path "..\common\include\options.h")) {
        Write-Host "Error: Required header files not found in ..\common\include\" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "Setting up test environment..." -ForegroundColor Yellow
    Write-Host "Test directory: $PWD"
    Write-Host "Root directory: $((Get-Item '..').FullName)"
    Write-Host ""
    
    # Remove old executable if it exists
    if (Test-Path "bootgen_tests.exe") {
        Remove-Item "bootgen_tests.exe" -Force
    }
    
    Write-Host "Building tests..." -ForegroundColor Yellow
    
    $buildSuccess = $false
    
    # Determine compiler to use
    switch ($Compiler.ToLower()) {
        "gcc" {
            if (Test-Command "g++") {
                $buildSuccess = Build-WithGCC $Verbose
            } else {
                Write-Host "Error: g++ not found in PATH" -ForegroundColor Red
                exit 1
            }
        }
        "cl" {
            if (Test-Command "cl") {
                $buildSuccess = Build-WithCL $Verbose
            } else {
                Write-Host "Error: cl not found in PATH" -ForegroundColor Red
                exit 1
            }
        }
        "auto" {
            if (Test-Command "g++") {
                $buildSuccess = Build-WithGCC $Verbose
            } elseif (Test-Command "cl") {
                $buildSuccess = Build-WithCL $Verbose
            } else {
                Write-Host "Error: No suitable C++ compiler found in PATH" -ForegroundColor Red
                Write-Host "Please install one of the following:" -ForegroundColor Red
                Write-Host "- MinGW/MSYS2 (for g++)" -ForegroundColor Red
                Write-Host "- Visual Studio Build Tools (for cl)" -ForegroundColor Red
                exit 1
            }
        }
        default {
            Write-Host "Error: Unknown compiler '$Compiler'" -ForegroundColor Red
            Write-Host "Valid options: gcc, cl, auto" -ForegroundColor Red
            exit 1
        }
    }
    
    if (-not $buildSuccess) {
        Write-Host "Error: Compilation failed" -ForegroundColor Red
        exit 1
    }
    
    if (-not (Test-Path "bootgen_tests.exe")) {
        Write-Host "Error: Test executable was not created" -ForegroundColor Red
        exit 1
    }
    
    Write-Host ""
    Write-Host "Tests compiled successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Running tests..." -ForegroundColor Yellow
    Write-Host "========================================" -ForegroundColor Cyan
    
    # Run the tests
    $testProcess = Start-Process -FilePath ".\bootgen_tests.exe" -Wait -PassThru -NoNewWindow
    $exitCode = $testProcess.ExitCode
    
    Write-Host "========================================" -ForegroundColor Cyan
    
    if ($exitCode -eq 0) {
        Write-Host "Tests completed successfully!" -ForegroundColor Green
    } else {
        Write-Host "Tests failed with exit code: $exitCode" -ForegroundColor Red
    }
    
    Write-Host ""
    exit $exitCode
}

# Run the main function
Main
