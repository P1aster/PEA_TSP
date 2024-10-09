#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>

bool FileManager::ensureFileExists(const std::string& filePath) {
    if (std::filesystem::exists(filePath)) {
        return true;
    }

    try {
        // Create directories if they don't exist
        std::filesystem::create_directories(std::filesystem::path(filePath).parent_path());

 
        std::ofstream file(filePath);
        if (file) {
            file.close();
            return true;
        }
        else {
            std::cerr << "Failed to create file: " << filePath << std::endl;
            return false;
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    }
}