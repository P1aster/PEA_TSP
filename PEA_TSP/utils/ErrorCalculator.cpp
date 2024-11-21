#include "ErrorCalculator.h"
#include <cmath> 

ErrorCalculator::ErrorCalculator(std::optional<double> trueValue) {
	this->trueValue = trueValue.value_or(0.0);
}

double ErrorCalculator::calculateAbsoluteError(double measuredValue) {
    return std::abs(measuredValue - trueValue);
}

double ErrorCalculator::calculateRelativeError(double measuredValue) {
    return (calculateAbsoluteError(measuredValue) / trueValue) * 100.0;
}

void ErrorCalculator::setTrueValue(double trueValue) {
	this->trueValue = trueValue;
}
