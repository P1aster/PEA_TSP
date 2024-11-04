#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <optional>

class Config {
private:
    std::string inputPath;
    std::string outputPath;
    std::string configFilePath;
    bool checkAllNodes;
    std::vector<int> nodeList;
    int repeatNumber = 0;
    int coutFlag = 0;
    std::optional<std::ofstream> outputFileOpt;
    bool ensureExists(std::string path);
    bool createPath(std::string path);

public:
    Config(std::string configFilePath);
    bool readConfig();
    std::string getInputPath();
    std::string getOutputPath();
    int getRepeatNumber();
    int getCoutFlag();
	bool getCheckAllNodes();
	std::vector<int> getNodeList();

    void cout(std::string str);
    bool checkPathIsFile(std::string path);

    bool openOutputFile();
    void writeToOutputFile(const std::string& content);
    void closeOutputFile();
};
