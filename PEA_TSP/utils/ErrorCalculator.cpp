#include "ErrorCalculator.h"
#include <cmath> // Include cmath for std::abs

ErrorCalculator::ErrorCalculator(std::optional<double> trueValue) {
	this->trueValue = trueValue.value_or(0.0);
}

double ErrorCalculator::calculateAbsoluteError(double measuredValue) const {
    return std::abs(measuredValue - trueValue);
}

double ErrorCalculator::calculateRelativeError(double measuredValue) const {
    return (calculateAbsoluteError(measuredValue) / trueValue) * 100.0;
}

void ErrorCalculator::setTrueValue(double trueValue) {
	this->trueValue = trueValue;
}
