#pragma once
#include <vector>
#include "structures/TSP_Result.h"
#include "structures/Graph.h"
#include <random>
#include "CoolingSchema.h"


class SA
{
private:
	Graph graph;
	int nodesNumber;
	int** matrix;

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

public:
	SA(Graph graph);
	TSP_Result run( 
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
	int calculateCost(std::vector<int>& curr);
};

