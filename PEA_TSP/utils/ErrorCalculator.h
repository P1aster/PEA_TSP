#pragma once
#include <optional>

class ErrorCalculator {
public:
    ErrorCalculator(std::optional<double> trueValue = 0);

    double calculateAbsoluteError(double measuredValue) const;

    double calculateRelativeError(double measuredValue) const;

	void setTrueValue(double trueValue);

private:
    double trueValue;
};

