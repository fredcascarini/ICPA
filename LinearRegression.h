#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include <vector>

template <typename T>
std::vector<T> GetLinearFit(const std::vector<T>& data, T r2tol, T nSegSize, T maxSegNum, double start);

template <typename T>
std::vector<T> LinRegress(const std::vector<T>& xdata, const std::vector<T>& ydata, double min, double max);

#endif
