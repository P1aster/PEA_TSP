#include <iostream>
#include <fstream>
#include <filesystem>


class FileManager {
    public:
        bool ensureFileExists(const std::string& filePath);
};

