#include "SA.h"
#include "algorithms/RNN.h"
#include "utils/Time.h"
#include "algorithms/R.h"


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

}

TSP_Result SA::run(double initialTemperature, double finalTemperature, double coolingRate, std::optional<CoolingSchema> coolingSchema, std::optional<int> patience, std::optional<std::string> initialPathMethod, std::optional<int> maxDurationMs, std::optional<int> knownMinPathCost) {
	
	this->initialTemperature = initialTemperature; // Starting temperature for the algorithm.
	this->finalTemperature = finalTemperature;     // Temperature at which the algorithm terminates.
	this->coolingRate = coolingRate;               // Rate at which the temperature decreases.

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


	
	TSP_Result result;

	if (initialPathMethod.has_value() && initialPathMethod == "RNN") {
		// Generate an initial solution using the Repeated Nearest Neighbour heuristic.
		RNN rnn(graph);
		result = rnn.findRepeatedNearestNaighbour();
	}
	else {
		// Generate an initial solution using the Repeated Nearest Neighbour heuristic.
		R r(graph);
		result = r.findFirstRandomHamiltonianCircle();
	}

	simulatedAnnealing(result);

	result.bestPath = bestPath;         // Best path found during the execution.
	result.minPathCost = minPathCost;   // Total cost of the best path.
	return result;

}

/*
	Core function implementing the Simulated Annealing algorithm.
	Iteratively explores the solution space to find a near-optimal solution to the TSP.
*/
void SA::simulatedAnnealing(TSP_Result initial_result) {
	Time time;
	
	// Track iterations since last improvement for patience mechanism
	int iterationsSinceLastImprovement = 0;
	bool shouldTerminate = false;


	int currentPathCost;
	std::vector<int> currentPath;

	double temperature = initialTemperature;

	currentPath = initial_result.bestPath;
	bestPath = initial_result.bestPath;
	currentPathCost = initial_result.minPathCost;
	minPathCost = initial_result.minPathCost;
	
	time.start();
	// Main loop: continues until the system has cooled down to the final temperature.
	while (temperature > finalTemperature && !shouldTerminate) {
		int epochLength = std::max(2, static_cast<int>(nodesNumber * (temperature / initialTemperature)));
		bool improvementInEpoch = false;

		for (int i = 0; i < epochLength; i++) {
			// Generate a new solution by swapping two random nodes in the current path.
			std::vector<int> newPath = determinNewSolution(currentPath);
			int newPathCost = calculateCost(newPath);

			// Decide whether to accept the new solution based on the acceptance probability.
			if (accept(currentPathCost, newPathCost, temperature)) {

				if (currentPathCost < newPathCost) {
					iterationsSinceLastImprovement = 0;
					improvementInEpoch = true;
				}

				currentPath = newPath;
				currentPathCost = newPathCost;


				// Update the best solution found if the new path is better (has lower cost).
				if (currentPathCost < minPathCost) {
					bestPath = currentPath;
					minPathCost = currentPathCost;
					
				}
			}
			

			if (knownMinPathCost.has_value() && currentPathCost <= knownMinPathCost.value()) {
				shouldTerminate = true;
				break;
			}

			if (maxDurationMs.has_value()) {
				float elapsedTime = time.getElapsedTimeNow();
				if (elapsedTime >= maxDurationMs.value()) {
					shouldTerminate = true;
					break;
				}
			}
		}

		if (!improvementInEpoch) {
			iterationsSinceLastImprovement += epochLength;
		}

		if (patience.has_value() && iterationsSinceLastImprovement >= patience.value()) {

			temperature = std::min(temperature * 1.5, initialTemperature);
			iterationsSinceLastImprovement = 0;
			
		}
		else {
			// Cool down the temperature according to the selected cooling schema.
			switch (coolingSchema) {
			case CoolingSchema::Linear:
				temperature = std::max(temperature - coolingRate, finalTemperature);
				break;
			case CoolingSchema::Exponential:
				temperature *= coolingRate;
				break;
			case CoolingSchema::Logarithmic:
				temperature /= (1 + coolingRate * log(1 + temperature));
				break;
			}
		}
	}
}

/*
	Calculates the total cost of a given path.
	Sums the distances between consecutive nodes and returns to the starting node.
*/
int SA::calculateCost(std::vector<int>& curr) {
	int cost = 0;
	for (int i = 0; i < nodesNumber - 1; ++i) {
		cost += matrix[curr[i]][curr[i + 1]];
	}

	// Add the distance from the last node back to the first one, to complete the circle.
	cost += matrix[curr[nodesNumber - 1]][curr[0]];
	return cost;
}


// Generates a new candidate solution by performing a random swap of two nodes in the current path.
std::vector<int> SA::determinNewSolution(const std::vector<int>& currPath) {
	std::uniform_int_distribution<> dis(0, nodesNumber - 1);
	std::uniform_int_distribution<> method_dis(0, 1); // Randomly choose between swap and insertion
	std::vector<int> newPath = currPath;

	if (method_dis(gen) == 0) {
		// Swap method
		int i = dis(gen);
		int j = dis(gen);
		while (i == j) {
			j = dis(gen);
		}
		std::swap(newPath[i], newPath[j]);
	}
	else {
		// Insertion method
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
