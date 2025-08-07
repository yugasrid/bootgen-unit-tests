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

#ifndef MOCK_CLASSES_H
#define MOCK_CLASSES_H

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <cstring>  // For memset, strcmp, strlen, strcpy
#include <cstdio>   // For printf

// Mock Options class for testing
class MockOptions {
public:
    std::string bifFileName;
    std::string outputFileName;
    std::string architecture;
    bool parseArgsCalled = false;
    bool processVerifyKDFCalled = false;
    bool processReadImageCalled = false;
    bool helpRequested = false;
    bool verboseMode = false;
    std::vector<std::string> arguments;

    void ParseArgs(int argc, const char* argv[]) {
        parseArgsCalled = true;
        arguments.clear();
        
        // Simple argument parsing for testing
        for (int i = 0; i < argc; i++) {
            arguments.push_back(std::string(argv[i]));
        }
        
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            
            if (arg == "-image" && i + 1 < argc) {
                bifFileName = argv[i + 1];
                i++; // Skip next argument
            }
            else if (arg == "-o" && i + 1 < argc) {
                outputFileName = argv[i + 1];
                i++; // Skip next argument
            }
            else if (arg == "-arch" && i + 1 < argc) {
                architecture = argv[i + 1];
                i++; // Skip next argument
            }
            else if (arg == "-help" || arg == "--help" || arg == "-h") {
                helpRequested = true;
            }
            else if (arg == "-verbose" || arg == "-v") {
                verboseMode = true;
            }
        }
    }

    void ProcessVerifyKDF() {
        processVerifyKDFCalled = true;
    }

    void ProcessReadImage() {
        processReadImageCalled = true;
    }

    std::string GetBifFilename() const {
        return bifFileName;
    }
    
    std::string GetOutputFilename() const {
        return outputFileName;
    }
    
    std::string GetArchitecture() const {
        return architecture;
    }
    
    bool IsHelpRequested() const {
        return helpRequested;
    }
    
    bool IsVerboseMode() const {
        return verboseMode;
    }
    
    // Reset for clean testing
    void Reset() {
        bifFileName.clear();
        outputFileName.clear();
        architecture.clear();
        parseArgsCalled = false;
        processVerifyKDFCalled = false;
        processReadImageCalled = false;
        helpRequested = false;
        verboseMode = false;
        arguments.clear();
    }
};

// Mock BIF_File class for testing
class MockBIF_File {
public:
    std::string filename;
    bool processCalled = false;
    bool isValid = true;
    std::string errorMessage;

    explicit MockBIF_File(const std::string& fname) : filename(fname) {
        if (fname.empty()) {
            isValid = false;
            errorMessage = "Empty filename provided";
        }
        else if (fname.length() > 1000) {
            isValid = false;
            errorMessage = "Filename too long";
        }
        else if (fname.find("invalid") != std::string::npos) {
            isValid = false;
            errorMessage = "Invalid filename pattern";
        }
    }

    void Process(MockOptions& options) {
        processCalled = true;
        
        if (!isValid) {
            throw std::runtime_error("Cannot process invalid BIF file: " + errorMessage);
        }
        
        if (filename.find("throw") != std::string::npos) {
            throw std::runtime_error("Simulated processing error");
        }
    }
    
    bool IsValid() const {
        return isValid;
    }
    
    std::string GetErrorMessage() const {
        return errorMessage;
    }
};

// Simplified BootGenApp for testing
class TestableBootGenApp {
public:
    MockOptions* mockOptions = nullptr;
    MockBIF_File* mockBifFile = nullptr;
    bool displayBannerCalled = false;

    void DisplayBanner() {
        displayBannerCalled = true;
        // Simulate banner display
    }

    void Run(int argc, const char* argv[]) {
        DisplayBanner();
        
        MockOptions options;
        if (mockOptions) {
            options = *mockOptions;
        }
        
        options.ParseArgs(argc, argv);
        
        if (options.IsHelpRequested()) {
            // Help mode - no further processing needed
            return;
        }
        
        options.ProcessVerifyKDF();
        options.ProcessReadImage();
        
        std::string bifFile = options.GetBifFilename();
        if (!bifFile.empty()) {
            MockBIF_File bif(bifFile);
            if (mockBifFile) {
                bif = *mockBifFile;
            }
            bif.Process(options);
        }
    }
    
    // Test helper methods
    void SetMockOptions(MockOptions* opts) {
        mockOptions = opts;
    }
    
    void SetMockBifFile(MockBIF_File* bif) {
        mockBifFile = bif;
    }
    
    bool WasDisplayBannerCalled() const {
        return displayBannerCalled;
    }
};

// Realistic implementations with intentional bugs for rigorous testing
class RealisticOptions {
public:
    char* bifFileName = nullptr;  // Potential memory issues
    char outputFileName[256];     // Fixed buffer - potential overflow
    std::string architecture;
    bool parseArgsCalled = false;
    bool processVerifyKDFCalled = false;
    bool processReadImageCalled = false;

    RealisticOptions() {
        memset(outputFileName, 0, sizeof(outputFileName));
    }
    
    ~RealisticOptions() {
        if (bifFileName) {
            delete[] bifFileName;  // Could be double-deleted
        }
    }

    void ParseArgs(int argc, const char* argv[]) {
        parseArgsCalled = true;
        
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-image") == 0 && i + 1 < argc) {
                // Potential buffer overflow - no size checking
                const char* fname = argv[i + 1];
                bifFileName = new char[strlen(fname) + 1];
                strcpy(bifFileName, fname);
                i++;
            }
            else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                // Buffer overflow potential - this is an intentional bug for testing
                // In real code, should use strncpy or check length first
                if (strlen(argv[i + 1]) >= sizeof(outputFileName)) {
                    // Simulate detection of potential overflow
                    throw std::runtime_error("Output filename too long - potential buffer overflow");
                }
                strcpy(outputFileName, argv[i + 1]);
                i++;
            }
        }
    }

    void ProcessVerifyKDF() {
        processVerifyKDFCalled = true;
        if (bifFileName == nullptr) {
            throw std::runtime_error("No BIF file specified");
        }
    }

    void ProcessReadImage() {
        processReadImageCalled = true;
    }

    const char* GetBifFilename() const {
        return bifFileName;  // Could return nullptr
    }
    
    void Reset() {
        if (bifFileName) {
            delete[] bifFileName;
            bifFileName = nullptr;
        }
        memset(outputFileName, 0, sizeof(outputFileName));
        architecture.clear();
        parseArgsCalled = false;
        processVerifyKDFCalled = false;
        processReadImageCalled = false;
    }
};

class RealisticBIF_File {
public:
    std::string filename;
    bool processCalled = false;

    explicit RealisticBIF_File(const std::string& fname) : filename(fname) {
        // No validation - potential issues
    }

    void Process(RealisticOptions& options) {
        processCalled = true;
        
        // Potential null pointer dereference - check for null first
        const char* bifName = options.GetBifFilename();
        if (bifName == nullptr) {
            throw std::runtime_error("No BIF filename provided");
        }
        
        if (strlen(bifName) > 10000) {
            throw std::runtime_error("Filename too long for processing");
        }
        
        // Simulate processing that could fail for certain files
        if (filename.find("crash") != std::string::npos) {
            throw std::runtime_error("Simulated crash in file processing");
        }
    }
};

class RealisticBootGenApp {
public:
    RealisticOptions* options = nullptr;
    char* buffer = nullptr;

    RealisticBootGenApp() {
        options = new RealisticOptions();
        buffer = new char[1024];
    }

    ~RealisticBootGenApp() {
        if (options) {
            delete options;
            // Potential double delete if called again
        }
        if (buffer) {
            delete[] buffer;
        }
    }

    void DisplayBanner() {
        // Potential buffer overflow - intentional bug for testing
        char version[64];
        // This string is exactly 63 characters to fit in buffer with null terminator
        strcpy(version, "BOOTGEN v2023.1.0.1234567890123456789012345678901234567890123");
        
        // Use the buffer
        printf("Version: %s\n", version);
    }

    void Run(int argc, const char* argv[]) {
        DisplayBanner();
        
        if (!options) {
            throw std::runtime_error("Internal error: options not initialized");
        }
        
        options->ParseArgs(argc, argv);
        options->ProcessVerifyKDF();
        options->ProcessReadImage();
        
        const char* bifFile = options->GetBifFilename();
        if (bifFile && strlen(bifFile) > 0) {
            RealisticBIF_File bif(bifFile);
            bif.Process(*options);
        }
    }
};

#endif // MOCK_CLASSES_H
