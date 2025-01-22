#pragma once
#include <vector>
#include "structures/TSP_Result.h"
#include "structures/Graph.h"
#include <random>
#include "CoolingSchema.h"
#include "MeasureUnitObject.h"
#include "utils/time.h"


class SA
{
private:
	Graph graph;
	int nodesNumber;
	int** matrix;

	Time timer;
	std::vector<MeasureUnitObject> measureResults;

	std::vector<int> bestPath;
	int minPathCost;
	std::optional<int> knownMinPathCost;
	std::optional<int> patience;
	std::optional<int> maxDurationMs;
	CoolingSchema coolingSchema;
	double initialTemperature;
	double finalTemperature;
	double coolingRate;

	std::random_device rd;
	std::mt19937 gen;

	bool accept(int currCost, int newCost, double temperature);
	std::vector<int> determinNewSolution(const std::vector<int>& current_solution);
	int calculateCost(const std::vector<int>& curr);


public:
	SA(Graph graph);
	ExtendedTSP_Result run(
		double initialTemperature,
		double finalTemperature, 
		double coolingRate, 
		std::optional<CoolingSchema> coolingSchema = std::nullopt,
		std::optional<int> patience = std::nullopt, 
		std::optional<std::string> initialPathMethod = std::nullopt, 
		std::optional<int> maxDurationMs = std::nullopt, 
		std::optional<int> knownMinPathCost = std::nullopt
	);
	void simulatedAnnealing(TSP_Result initial_result);
	double calculateInitialTemperature(double acceptanceRatio, int numSamples);
};

