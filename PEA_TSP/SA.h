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
	CoolingSchema coolingSchema;
	double initialTemperature;
	double finalTemperature;
	double coolingRate;

	std::random_device rd;
	std::mt19937 gen;


	bool accept(int currCost, int newCost, double temperature);
	std::vector<int> determinNewSolution(const std::vector<int>& current_solution);

public:
	SA(Graph graph, CoolingSchema schema);
	TSP_Result run( double initialTemperature, double finalTemperature, double coolingRate);
	void simulatedAnnealing();
	int calculateCost(std::vector<int>& curr);
};

