#include "SA.h"
#include "algorithms/RNN.h"
#include "utils/Time.h"
#include "algorithms/R.h"
#include <numeric>


/*
	Constructor for the Simulated Annealing(SA) algorithm.
	Initializes the graph, cooling schema, number of nodes, and adjacency matrix.
*/
SA::SA(Graph graph) {
	this->graph = graph;
	this->minPathCost = INT_MAX;
	this->knownMinPathCost;
	this->initialTemperature = 100000;
	this->finalTemperature = 0.0001;
	this->coolingRate = 0.995;
	this->patience = 10000;
    this->coolingSchema = CoolingSchema::Exponential;
	this->nodesNumber = graph.getNodesNumber();
	this->matrix = graph.getMatrix();
	this->measureResults.clear();
	std::mt19937 gen(rd());
}

ExtendedTSP_Result SA::run(double initialTemperature, double finalTemperature, double coolingRate, std::optional<CoolingSchema> coolingSchema, std::optional<int> patience, std::optional<std::string> initialPathMethod, std::optional<int> maxDurationMs, std::optional<int> knownMinPathCost) {
	
	this->initialTemperature = initialTemperature; // Starting temperature for the algorithm.
	this->finalTemperature = finalTemperature;     // Temperature at which the algorithm terminates.
	this->coolingRate = coolingRate;               // Rate at which the temperature decreases.

	if (coolingSchema.has_value()) {
		this->coolingSchema = coolingSchema.value();
	}
	else {
		this->coolingSchema = CoolingSchema::Exponential;
	}


	if (knownMinPathCost.has_value()) {
		this->knownMinPathCost = knownMinPathCost.value();
	}

	if (maxDurationMs.has_value()) {
		this->maxDurationMs = maxDurationMs.value();
	}
	else {
		this->maxDurationMs = std::nullopt;
	}

	if (patience.has_value()) {
		this->patience = patience.value();
	}
	else {
		this->patience = 1000;
	}

	double calculatedInitialTemperature = calculateInitialTemperature(0.75, 100);
	
	TSP_Result result;

	timer.start();

	if (initialPathMethod.has_value() && initialPathMethod == "NN") {
		// Generate an initial solution using the Nearest Neighbour heuristic.
		RNN rnn(graph);
		result = rnn.findNearestNaighbour(0);
	}
	else {
		// Generate an initial solution using the Random heuristic.
		R r(graph);
		result = r.findFirstRandomHamiltonianCircle();
	}
	MeasureUnitObject res;
	res.cost = result.minPathCost;
	res.duration = timer.getElapsedTimeNow();
	measureResults.push_back(res);

	simulatedAnnealing(result);

	ExtendedTSP_Result extendedResult;

	extendedResult.instructedInitailTemp = calculatedInitialTemperature;
	extendedResult.bestPath = bestPath;         // Best path found during the execution.
	extendedResult.minPathCost = minPathCost;   // Total cost of the best path.
	extendedResult.timeStampData = measureResults;
	return extendedResult;

}

/*
	Core function implementing the Simulated Annealing algorithm.
	Iteratively explores the solution space to find a near-optimal solution to the TSP.
*/
void SA::simulatedAnnealing(TSP_Result initial_result) {
	// Track iterations since last improvement for patience mechanism
	int iterationsSinceLastImprovement = 0;
	bool shouldTerminate = false;

	double temperature = initialTemperature;

	bestPath = initial_result.bestPath;
	minPathCost = initial_result.minPathCost;
	std::vector<int> currentPath = initial_result.bestPath;
	int currentPathCost = initial_result.minPathCost;



	// Main loop: continues until the system has cooled down to the final temperature.
	while (temperature > finalTemperature && !shouldTerminate) {
		bool improvementInPhase = false;
		int epochLength = std::max(2, static_cast<int>(nodesNumber * (temperature / initialTemperature)));
		
		for (int i = 0; i < epochLength; i++) {
			// Generate a new solution by swapping two random nodes in the current path.
			std::vector<int> newPath = determinNewSolution(currentPath);
			int newPathCost = calculateCost(newPath);

			// Decide whether to accept the new solution based on the acceptance probability.
			if (accept(currentPathCost, newPathCost, temperature)) {

				if (newPathCost < currentPathCost) {
					iterationsSinceLastImprovement = 0;
					improvementInPhase = true;

				}

				currentPath = newPath;
				currentPathCost = newPathCost;


				// Update the best solution found if the new path is better (has lower cost).
				if (currentPathCost < minPathCost) {
					bestPath = currentPath;
					minPathCost = currentPathCost;

					MeasureUnitObject res;
					res.cost = currentPathCost;
					res.duration = timer.getElapsedTimeNow();
					measureResults.push_back(res);
				}
			}
			

			if (knownMinPathCost.has_value() && currentPathCost <= knownMinPathCost.value()) {
				shouldTerminate = true;
				break;
			}

			if (maxDurationMs.has_value()) {
				float elapsedTime = timer.getElapsedTimeNow();
				if (elapsedTime >= maxDurationMs.value()) {
					shouldTerminate = true;
					break;
				}
			}
		}

		if (!improvementInPhase) {
			iterationsSinceLastImprovement++;
		}

		if (patience.has_value() && iterationsSinceLastImprovement >= patience.value()) {
			temperature = std::min(temperature * 1.5, initialTemperature);
			iterationsSinceLastImprovement = 0;
			
		}else {
			// Cool down the temperature according to the selected cooling schema.
			switch (coolingSchema) {
			case CoolingSchema::Linear:
				temperature = std::max(temperature - (1 - coolingRate), finalTemperature);
				break;
			case CoolingSchema::Exponential:
				temperature *= coolingRate;
				break;
			case CoolingSchema::Logarithmic:
				temperature /= (1 + (1 - coolingRate) * log(1 + temperature));
				break;
			}
		}
		
	}
}

// Updated calculateInitialTemperature using Thompson and Bilbro's approach
double SA::calculateInitialTemperature(double acceptanceProbability = 0.75, int populationSize = 100) {
	// Generate a population of random solutions and compute their costs
	R r(graph);
	TSP_Result randomResult;
	std::vector<int> costs(populationSize);
	for (int i = 0; i < populationSize; ++i) {
		randomResult = r.findFirstRandomHamiltonianCircle();
		costs[i] = randomResult.minPathCost;
	}

	// Calculate mean and standard deviation of costs
	double meanCost = std::accumulate(costs.begin(), costs.end(), 0.0) / costs.size();
	double variance = 0.0;
	for (double cost : costs) {
		variance += std::pow(cost - meanCost, 2);
	}
	double stddev = std::sqrt(variance / costs.size());

	// Calculate \Delta E as mean + std deviation
	double deltaE = meanCost + stddev;

	return deltaE / std::log(1 / acceptanceProbability);
}


/*
	Calculates the total cost of a given path.
	Sums the distances between consecutive nodes and returns to the starting node.
*/
int SA::calculateCost(const std::vector<int>& curr) {
	int cost = 0;
	for (int i = 0; i < nodesNumber - 1; ++i) {
		cost += matrix[curr[i]][curr[i + 1]];
	}

	// Add the distance from the last node back to the first one, to complete the circle.
	cost += matrix[curr[nodesNumber - 1]][curr[0]];
	return cost;
}

/*
Generates a new candidate solution by performing a random swap of two nodes in the current path.
*/
std::vector<int> SA::determinNewSolution(const std::vector<int>& currPath) {
	std::uniform_int_distribution<> dis(0, nodesNumber - 1);
	std::uniform_int_distribution<> method_dis(0, 1); // Randomly choose between swap and insertion
	std::vector<int> newPath = currPath;

	if (method_dis(gen) == 0) {
		// Swap method:
		// Randomly select two distinct nodes in the current path and swap their positions.
		// This operation maintains the path length while introducing a small perturbation,
		// helping the algorithm explore neighboring solutions in the search space.
		int i = dis(gen);
		int j = dis(gen);
		while (i == j) {
			j = dis(gen);
		}
		std::swap(newPath[i], newPath[j]);
	}
	else {
		// Insertion method:
		// Randomly select a node and reinsert it at a different position in the path.
		// This method can produce more significant changes than swapping, allowing the
		// algorithm to potentially escape local optima by exploring distant regions of
		// the solution space.
		int i = dis(gen);
		int j = dis(gen);
		while (i == j) {
			j = dis(gen);
		}
		if (i < j) {
			std::rotate(newPath.begin() + i, newPath.begin() + i + 1, newPath.begin() + j + 1);
		}
		else {
			std::rotate(newPath.begin() + j, newPath.begin() + i, newPath.begin() + i + 1);
		}
	}

	return newPath;
}


/*
	Determines whether to accept the new solution based on the Metropolis criterion.
	Accepts better solutions or probabilistically accepts worse solutions based on temperature.
*/
bool SA::accept(int currCost, int newCost, double temperature) {
	if (newCost < currCost) {
		return true; // Always accept if the new solution is better (lower cost).
	} else {
		// Calculate acceptance probability for worse solutions.
		std::uniform_real_distribution<> prob_dist(0.0, 1.0);
		double probability = std::exp((currCost - newCost) / temperature);
		// Accept the worse solution with a probability that decreases as temperature decreases.
		return prob_dist(gen) < probability;
	}
}


