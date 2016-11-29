#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include <vector>

std::vector<double> GetLinearFit(std::vector<double> data, double r2tol, double nSegSize, double start);
/*
	Returns linear fit to set of data

	Input:
	data		:	vector of doubles, input data
	r2tol		:	tolerance in r^2
	nSegSize	:	size of minimum segment of data to be fitted
	start		:	index of initial point in data to work from

	Output:
	std::vector<double> res
	res[0]		:	end of linear segment
	res[1]		:	slope of linear segment
	res[2]		:	intercept of linear segment

*/

std::vector<double> LinRegress(std::vector<double> xdata, const std::vector<double>& ydata, double min, double max);
/*
	Uses a boost accumulator to optimise linear regression of input data

	Input:
	xdata		:	data to be analysed (x coordinates)
	ydata		:	data to be analysed (y coordinates)
	min			:	lower index to analyse from
	max			:	upper index to analyse to

	Output
	std::vector<double> result
	result[0]	:	slope of linear segment
	result[1]	:	intercept of linear segment
	result[2]	:	r value of linear segment
*/

std::vector<double> GausKern(double sigma, int width);
/*
	Returns a normalised 1D gaussian kernel for use in gaussian blurring. Mean is taken as centrepoint of kernel

	Input:
	sigma		:	standard deviation of gaussian
	width		:	1D length of kernel to return

	Output:
	std::vector<double> kernel
*/


std::vector<double> GausBlur(std::vector<double> data, int width, double sigma);
/*
	Blurs a set of data by successive Gaussian kernels

	Input:
	data		:	data to blur
	width		:	width of kernels to use
	sigma		:	standard deviation of kernels to use

	Output:
	std::vector<double> Result
*/
#endif

