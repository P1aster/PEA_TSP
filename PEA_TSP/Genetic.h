#pragma once
#include "Path.h"
#include <algorithm>
#include <random>
#include <optional>
#include "structures/TSP_Result.h"
#include "MeasureUnitObject.h"
#include "utils/time.h"
class Genetic
{
	private:
		Graph graph;
		int** matrix;
		int nodesNumber;
		std::random_device rd;
		std::mt19937 gen;

		Time timer;
		std::vector<MeasureUnitObject> measureResults;

		std::optional<int> knownMinPathCost;
		std::optional<int> maxDurationMs;

		Path bestSolution;


		Path mutate_swap(Path path);                             
		Path mutate_invert(Path path);

		Path selection_turnament(const std::vector<Path>& population);

		Path crossover(const Path& parent1, const Path& parent2);

		Path generateRandomPath();
		std::vector<Path> generateInitialPopulation(int populationSize);

		void solve(int mu, int lambda, int generations, double mutationRate, double crossoverRate);



	public:

		Genetic(Graph graph);
		ExtendedTSP_Result run(int mu, int lambda, int generations, double mutationRate, double crossoverRate, std::optional<int> knownMinPathCost = std::nullopt, std::optional<int> maxDurationMs = std::nullopt);


};
