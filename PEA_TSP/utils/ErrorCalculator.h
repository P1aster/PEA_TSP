#pragma once
#include <optional>

class ErrorCalculator {
public:
    ErrorCalculator(std::optional<double> trueValue = 0);

    double calculateAbsoluteError(double measuredValue);

    double calculateRelativeError(double measuredValue);

	void setTrueValue(double trueValue);

private:
    double trueValue;
};

