#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"
#include "LinearRegression.h"
#include <cmath>
#include <vector>
#include <iostream>

#define BOOST_NUMERIC_FUNCTIONAL_STD_VECTOR_SUPPORT
#include "boost_1_62_0\boost\accumulators\numeric\functional\vector.hpp"
#include "boost_1_62_0\boost\accumulators\accumulators.hpp"
#include "boost_1_62_0\boost\accumulators\statistics\stats.hpp"
#include "boost_1_62_0\boost\accumulators\statistics\variance.hpp"
#include "boost_1_62_0\boost\accumulators\statistics\covariance.hpp"
#include "boost_1_62_0\boost\accumulators\statistics\variates\covariate.hpp"
#include "boost_1_62_0\boost\math\distributions\normal.hpp"

using namespace boost::accumulators;

//following translated from python
std::vector<double> GetLinearFit(std::vector<double> data, double r2tol, double nSegSize, double start)
{
	std::vector<double> xData;
	std::vector<double> res;
	std::vector<double> linRegResult;

	double slope;
	double intercept;
	double r2;
	double nData = static_cast<double>(data.size());
	double iLinEnd;
	double segEnd;

	iLinEnd = 0;

	if (r2tol <= 0.0) {
		r2tol = 0.01;
	}
	else if (r2tol > 1.0) {
		r2tol = 1;
	}

	if (nSegSize > nData) {
		nSegSize = nData;
	}

	double nSegNum = (nData / nSegSize);

	for (unsigned long i = 0; i < data.size(); i++)
	{
		xData.push_back(static_cast<double>(i));
	}

	for (long i = 0; i <= nSegNum; i++)
	{
		iLinEnd += nSegSize;

		segEnd = (start + iLinEnd < data.size()) ? (start + iLinEnd) : data.size() - 1;

		linRegResult = LinRegress(xData, data, start, segEnd);

		r2 = linRegResult[2];

		if (r2 * r2 >= r2tol) {
			slope = linRegResult[0];
			intercept = linRegResult[1];
		}

		else if (segEnd == data.size() - 1) {
			break;
		}

		else {
			iLinEnd = (iLinEnd != nSegSize) ? (iLinEnd - nSegSize) : 1;
			break;

		}
	}

	res.push_back(iLinEnd);
	res.push_back(slope);
	res.push_back(intercept);

	return res;
}

//following translated from python's scipy library
std::vector<double> LinRegress(std::vector<double> xdata, const std::vector<double>& ydata, double min, double max) 
{
	accumulator_set<double, stats<tag::mean, tag::variance, tag::covariance<double, tag::covariate1> > > x_acc;
	accumulator_set<double, stats<tag::mean, tag::variance, tag::covariance<double, tag::covariate1> > > y_acc;


	for (unsigned long i = static_cast<unsigned long>(min); i <= static_cast<unsigned long>(max); ++i) {
		x_acc(xdata[i-2], covariate1 = ydata[i]);
		y_acc(ydata[i], covariate1 = xdata[i-2]);
	}

	std::vector<double> result;

	double xmean = mean(x_acc);
	double ymean = mean(y_acc);

	double ssxm = variance(x_acc);
	double ssym = variance(y_acc);

	double ssxym = covariance(x_acc);	

	double r;
	double r_num = ssxym;
	double r_den = sqrt(ssxm * ssym);

	double slope;
	double intercept;

	if (r_den == 0.0) {
		r = 0.0;
	}
	else {
		r = r_num / r_den;
		if (r > 1.0) {
			r = 1.0;
		}
		else if (r < -1.0) {
			r = -1.0;
		}
	}
	
	slope = r_num / ssxm;
	intercept = ymean - slope*xmean;

	result.push_back(slope);
	result.push_back(intercept);
	result.push_back(r);

	return result;
}


std::vector<double> GausKern(double sigma, int width) 
{

	std::vector<double> kernel;
	double mean = width / 2.0;
	double sum = 0.0;
	boost::math::normal normal_function(mean, sigma);
	for (int x = 0; x < width; ++x)
		{
			double x_pdf = pdf(normal_function, x);
			kernel.push_back(x_pdf);
			sum += x_pdf;
		}

	for (int x = 0; x < width; ++x) { kernel[x] /= sum;}

	return kernel;
}

std::vector<double> GausBlur(std::vector<double> data, int width, double sigma)
{
	std::vector<double> GK =  GausKern(sigma, width);
	std::vector<double> Result;

	for (unsigned long i = 0; i < data.size(); ++i) {
		double value = 0;
		for (unsigned long ii = i - width; ii <= i + width; ++i) {
			if (ii < 0) { continue; }
			else { value += data[ii] * GK[ii]; }
		}
		Result.push_back(value);
	}

	return Result;
}

