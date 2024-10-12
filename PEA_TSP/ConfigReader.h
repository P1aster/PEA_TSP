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
    std::optional<std::ofstream> outputFileOpt;
    bool ensureExists(std::string path);
    bool createPath(std::string path);

public:
    ConfigReader(std::string configFilePath);
    bool readConfig();
    std::string getInputPath();
    std::string getOutputPath();
    int getRepeatNumber();
    int getCoutFlag();
    bool checkPathIsFile(std::string path);

    bool openOutputFile();
    void writeToOutputFile(const std::string& content);
    void closeOutputFile();
};
