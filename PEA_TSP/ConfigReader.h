#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>

class ConfigReader {
private:
    std::string inputPath;
    std::string outputPath;
    std::string configFilePath;
    int repeatNumber = 0;
    int coutFlag = 0;
    bool ensureExists(std::string path);
    bool createPath(std::string path);

public:
    ConfigReader(std::string configFilePath);
    bool readConfig();
    std::string getInputPath();
    std::string getOutputPath();
    bool checkPathIsFile(std::string path);
    int getRepeatNumber();
    int getCoutFlag();
    
};
