# PowerShell Test runner script for Bootgen unit tests
# This script discovers and runs all unit tests in the unit_tests directory

Write-Host "=======================================" -ForegroundColor Cyan
Write-Host "BOOTGEN UNIT TEST RUNNER" -ForegroundColor Cyan
Write-Host "=======================================" -ForegroundColor Cyan
Write-Host ""

# Create reports directory if it doesn't exist
if (-not (Test-Path "test_reports")) {
    New-Item -ItemType Directory -Path "test_reports" | Out-Null
}

# Initialize counters
$totalTests = 0
$passedTests = 0
$failedTests = 0
$testExecutables = @()

# Find all test executables in build directory
Write-Host "Discovering test executables..."
$buildPath = "..\build"
if (Test-Path $buildPath) {
    $testExecutables = Get-ChildItem -Path $buildPath -Name "test_*.exe" -ErrorAction SilentlyContinue
    foreach ($exe in $testExecutables) {
        Write-Host "  Found: $exe"
    }
} else {
    Write-Host "Build directory not found. Please run 'make unit-tests' first." -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Found $($testExecutables.Count) test executable(s)"
Write-Host ""

# Run each test executable
foreach ($testExe in $testExecutables) {
    $testName = [System.IO.Path]::GetFileNameWithoutExtension($testExe)
    $testPath = Join-Path $buildPath $testExe
    
    Write-Host "=======================================" -ForegroundColor Cyan
    Write-Host "Running: $testName" -ForegroundColor Blue
    Write-Host "=======================================" -ForegroundColor Cyan
    
    # Run the test and capture exit code
    $process = Start-Process -FilePath $testPath -NoNewWindow -Wait -PassThru
    
    if ($process.ExitCode -eq 0) {
        Write-Host "✓ $testName PASSED" -ForegroundColor Green
        $passedTests++
    } else {
        Write-Host "✗ $testName FAILED" -ForegroundColor Red
        $failedTests++
    }
    
    $totalTests++
    Write-Host ""
}

# Move individual test reports to reports directory
Write-Host "Moving test reports..."
Get-ChildItem -Name "*_report.txt" -ErrorAction SilentlyContinue | ForEach-Object {
    Move-Item $_ "test_reports\" -Force
}

# Calculate total individual tests from all reports
$totalIndividualTests = 0
$totalPassedIndividual = 0
$totalFailedIndividual = 0

Get-ChildItem "test_reports\*_report.txt" -ErrorAction SilentlyContinue | ForEach-Object {
    $content = Get-Content $_.FullName -ErrorAction SilentlyContinue
    if ($content) {
        # Extract test counts from individual reports
        $passedLine = $content | Where-Object { $_ -match "^Passed: (\d+)" }
        $failedLine = $content | Where-Object { $_ -match "^Failed: (\d+)" }
        $totalLine = $content | Where-Object { $_ -match "^Total Tests: (\d+)" }
        
        if ($passedLine -match "^Passed: (\d+)") {
            $totalPassedIndividual += [int]$matches[1]
        }
        if ($failedLine -match "^Failed: (\d+)") {
            $totalFailedIndividual += [int]$matches[1]
        }
        if ($totalLine -match "^Total Tests: (\d+)") {
            $totalIndividualTests += [int]$matches[1]
        }
    }
}

# Generate summary report
$summaryFile = "test_reports\SUMMARY_REPORT.txt"
Write-Host "Generating summary report: $summaryFile"

$successRate = if ($totalTests -gt 0) { [math]::Round(($passedTests * 100.0 / $totalTests), 1) } else { 0 }
$individualSuccessRate = if ($totalIndividualTests -gt 0) { [math]::Round(($totalPassedIndividual * 100.0 / $totalIndividualTests), 1) } else { 0 }

$summaryContent = @"
======================================
BOOTGEN UNIT TESTS - SUMMARY REPORT
======================================
Generated: $(Get-Date)

OVERALL RESULTS:
===============
Total Test Suites: $totalTests
Passed: $passedTests
Failed: $failedTests
Success Rate: $successRate%

DETAILED TEST RESULTS:
=====================
Total Individual Tests: $totalIndividualTests
Individual Tests Passed: $totalPassedIndividual
Individual Tests Failed: $totalFailedIndividual
Individual Success Rate: ${individualSuccessRate}%

TEST SUITE DETAILS:
==================
"@

# Add details for each test
foreach ($testExe in $testExecutables) {
    $testName = [System.IO.Path]::GetFileNameWithoutExtension($testExe)
    $testPath = Join-Path $buildPath $testExe
    
    $process = Start-Process -FilePath $testPath -NoNewWindow -Wait -PassThru -WindowStyle Hidden
    if ($process.ExitCode -eq 0) {
        $summaryContent += "`n$testName`: PASSED"
    } else {
        $summaryContent += "`n$testName`: FAILED"
    }
}

$summaryContent += @"

INDIVIDUAL REPORTS:
==================
"@

# List individual report files
Get-ChildItem -Path "test_reports" -Name "*_report.txt" -ErrorAction SilentlyContinue | ForEach-Object {
    $summaryContent += "`n- $_"
}

# Write summary to file
$summaryContent | Out-File -FilePath $summaryFile -Encoding UTF8

# Display final summary
Write-Host "=======================================" -ForegroundColor Cyan
Write-Host "FINAL SUMMARY" -ForegroundColor Cyan
Write-Host "=======================================" -ForegroundColor Cyan
Write-Host "Total Test Suites: $totalTests"
Write-Host "Passed: $passedTests" -ForegroundColor Green
Write-Host "Failed: $failedTests" -ForegroundColor Red

# Display individual test counts
Write-Host ""
Write-Host "INDIVIDUAL TEST DETAILS:" -ForegroundColor Cyan
Write-Host "========================" -ForegroundColor Cyan
Write-Host "Total Individual Tests: $totalIndividualTests" -ForegroundColor Blue
Write-Host "Individual Tests Passed: $totalPassedIndividual" -ForegroundColor Green
Write-Host "Individual Tests Failed: $totalFailedIndividual" -ForegroundColor Red
Write-Host "Individual Success Rate: ${individualSuccessRate}%" -ForegroundColor Cyan
Write-Host ""

if ($failedTests -eq 0) {
    Write-Host "🎉 All test suites passed!" -ForegroundColor Green
    $exitCode = 0
} else {
    Write-Host "❌ Some test suites failed." -ForegroundColor Red
    Write-Host "Check individual reports in test_reports/ directory" -ForegroundColor Yellow
    $exitCode = 1
}

Write-Host ""
Write-Host "Reports available in: test_reports/"
Write-Host "Summary report: $summaryFile"
Write-Host "=======================================" -ForegroundColor Cyan

exit $exitCode
