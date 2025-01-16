#pragma once
#include <vector>
#include <optional>
#include "../MeasureUnitObject.h"

struct TSP_Result {
    std::vector<int> bestPath;
    int minPathCost = INT_MAX;
};

struct ExtendedTSP_Result : public TSP_Result {
    std::vector<MeasureUnitObject> timeStampData;
    std::optional<double> instructedInitailTemp = std::nullopt;
};