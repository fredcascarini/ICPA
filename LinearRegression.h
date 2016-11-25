#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include <vector>

std::vector<double> GetLinearFit(std::vector<double> data, double r2tol, double nSegSize, double maxSegNum, double start);

std::vector<double> LinRegress(std::vector<double> xdata, const std::vector<double>& ydata, double min, double max);

std::vector<double> GausKern(double sigma, int width);

std::vector<double> GausBlur(std::vector<double> data, int width, double sigma);

#endif

