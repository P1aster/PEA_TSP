#include "Config.h"
#include <filesystem>
#include <nlohmann/json.hpp>


Config::Config(std::string configFilePath) {
    this->configFilePath = configFilePath;

}

bool Config::ensureExists(std::string path) {
    if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
        return true;
    }
    return false;
}

bool Config::createPath(std::string path) {
    try {
        // Create directories if they don't exist
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());

        // Create the file
        std::ofstream file(path);
        if (file) {
            file.close();
            return true;
        }
        else {
            std::cerr << "Failed to create file: " << path << std::endl;
            return false;
        }
    }
    catch (const std::filesystem::filesystem_error& error) {
        std::cerr << "Filesystem error: " << error.what() << std::endl;
        return false;
    }
}

bool Config::readConfig() {

    if (!ensureExists(configFilePath)) {
        std::cerr << "Config file does not exist and could not be created: " << configFilePath << std::endl;
        return false;
    }

    std::ifstream file(configFilePath);

    if (!file) {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        switch (lineNumber) {
        case 0:
            if (!ensureExists(line)) {
                std::cerr << "Input file or directory does not exist and could not be created: " << inputPath << std::endl;
                return false;
            }
            inputPath = line;
            break;
        case 1:
            if (!ensureExists(line) && !createPath(line)) {
                std::cerr << "Output file path does not exist and could not be created: " << outputPath << std::endl;
                return false;
            }
            outputPath = line;
            break;
        case 2:

            try {
                if (line.front() == '{' && line.back() == '}') {
                    nlohmann::json jsonObject = nlohmann::json::parse(line);
                    if (jsonObject.contains("checkAllNodes")) {
                        checkAllNodes = jsonObject["checkAllNodes"];
                    }
                    if (jsonObject.contains("nodeList")) {
                        nodeList = jsonObject["nodeList"].get<std::vector<int>>();
                    }
                    if (jsonObject.contains("knownMinPathCost")) {
                        knownMinPathCost = jsonObject["knownMinPathCost"];
                    }
                    if (jsonObject.contains("permutations")) {
                        permutations = jsonObject["permutations"];
                    }
                    if (jsonObject.contains("maxDuration")) {
                        maxDuration = jsonObject["maxDuration"];
                    }
                    if (jsonObject.contains("repeatNumber")) {
                        repeatNumber = jsonObject["repeatNumber"];
                    }
					if (jsonObject.contains("print")) {
						coutFlag = jsonObject["print"].get<bool>();
					}
                }
                else {
                    std::cerr << "Line is not in JSON format: " << line << std::endl;
                    return false;
                }
            }
            catch (nlohmann::json::parse_error& e) {
                std::cerr << "JSON Parse Error: " << e.what() << std::endl;
                return false;
            }
            break;
        default:
            break;
        }
        lineNumber++;
    }

    file.close();
    return true;
}

std::string Config::getInputPath() { return inputPath; }
std::string Config::getOutputPath() { return outputPath; }
int Config::getRepeatNumber() { return repeatNumber; };
bool Config::getCoutFlag() { return coutFlag; };
std::optional<int> Config::getKnownMinPathCost() { return knownMinPathCost; };
std::optional<int> Config::getPermutations() { return permutations; };
std::optional<int> Config::getMaxDuration() { return maxDuration; };
bool Config::getCheckAllNodes() { return checkAllNodes; };
std::vector<int> Config::getNodeList() { return nodeList; }

bool Config::openOutputFile() {
    outputFile.open(outputPath); // try open file
    if (outputFile.is_open()) {
        return true;
    } else {
        return false;
    }
}
void Config::writeToOutputFile(const std::string& content) {
    if (outputFile.is_open()) {
        outputFile << content;
    }
    else {
        std::cerr << "Output file is not open." << std::endl;
    }
}
void Config::closeOutputFile() {
    if (outputFile.is_open()) {
        outputFile.close();
	}
    else {
        std::cerr << "Output file is not open." << std::endl;
    }
}
void Config::cout(std::string str) {
    if (coutFlag) {
        std::cout << str;
    }
};


void Config::preprocessOutputFile(std::optional<int> knownMinPathCost, int nodesNumber) {

    this->writeToOutputFile("Input Path," + inputPath + "\n");
    this->writeToOutputFile("Nodes Number," + std::to_string(nodesNumber) + "\n");
    this->writeToOutputFile("Min path cost,");

    if (knownMinPathCost.has_value()) {
        this->writeToOutputFile(std::to_string(knownMinPathCost.value()));
    }
    

    this->writeToOutputFile("\nStartNode");
    for (int repeat = 0; repeat < repeatNumber; repeat++) {
        this->writeToOutputFile(",repeat " + std::to_string(repeat + 1));
    }
    this->writeToOutputFile(",Avg Time, Best Result, Avg Absolute Err, Avg Relative Err\n");

}

