#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H

#include <vector>

std::vector<double> get_linear_fit(std::vector<double> data, double r2_tol, int n_seg_size, int start);
/*
	Returns linear fit to set of data

	Input:
	data		:	vector of doubles, input data
	r2_tol		:	tolerance in r^2
	n_seg_size	:	size of minimum segment of data to be fitted
	start		:	index of initial point in data to work from

	Output:
	std::vector<double> res
	res[0]		:	end of linear segment
	res[1]		:	slope of linear segment
	res[2]		:	intercept of linear segment

*/

std::vector<double> lin_regress(std::vector<double> x_data, const std::vector<double>& y_data, int min, int max);
/*
	Uses a boost accumulator to optimise linear regression of input data

	Input:
	x_data		:	data to be analysed (x coordinates)
	y_data		:	data to be analysed (y coordinates)
	min			:	lower index to analyse from
	max			:	upper index to analyse to

	Output
	std::vector<double> result
	result[0]	:	slope of linear segment
	result[1]	:	intercept of linear segment
	result[2]	:	r value of linear segment
*/

std::vector<double> mvc_xy(std::vector<double> xdata, std::vector<double> ydata, int min, int max);

std::vector<double> gaus_kern(double sigma, int width);
/*
	Returns a normalised 1D gaussian kernel for use in gaussian blurring. Mean is taken as centrepoint of kernel

	Input:
	sigma		:	standard deviation of gaussian
	width		:	1D length of kernel to return

	Output:
	std::vector<double> kernel
*/


std::vector<double> gaus_blur(std::vector<double> data, int width, double sigma);
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

