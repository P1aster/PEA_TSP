#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <optional>
#include "../structures/Graph.h"
#include "../CoolingSchema.h"


class Config {
private:
    std::string inputPath;
    std::string outputPath;
    std::string configFilePath;

    bool checkAllNodes = false;
    std::vector<int> nodeList;

    int repeatNumber = 0;
    bool coutFlag = false;

	double initialTemperature = 0;
	double finalTemperature = 0;
	double coolingRate = 0;

    double mutationRate = 0.1;
    double crossoverRate = 1.0;
    int mu = 30;
    int lambda = 100;

	CoolingSchema coolingSchema = CoolingSchema::Exponential;
	std::string initialPathMethod = "RNN";

	int patience = 1000;
    std::optional<int> permutations = std::nullopt;
    std::optional<int> knownMinPathCost = std::nullopt;
    std::optional<int> maxDuration = std::nullopt;

    std::ofstream outputFile;

    bool ensureExists(std::string path);
    bool createPath(std::string path);

public:

    Config(std::string configFilePath);
    bool readConfig();
    std::string getInputPath();
    std::string getOutputPath();
    int getRepeatNumber();
    bool getCoutFlag();
    std::optional<int> getKnownMinPathCost();
    std::optional<int> getPermutations();
    std::optional<int> getMaxDuration();
    bool getCheckAllNodes();
    std::vector<int> getNodeList();

	double getInitialTemperature();
	double getFinalTemperature();
	double getCoolingRate();
	CoolingSchema getCoolingSchema();
	double getMutationRate();
    double getCrossoverRate();
	int getMu();
	int getLambda();
	std::string getInitialPathMethod();
    int getPatience();

    void cout(std::string str);

    bool openOutputFile();
    void closeOutputFile();
    void writeToOutputFile(const std::string& content);

    void preprocessOutputFile(std::optional <int> knownMinPathCost, int nodesNumber);

};
