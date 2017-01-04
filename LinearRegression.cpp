#include "LinearRegression.h"
#include <cmath>
#include <vector>

#define BOOST_NUMERIC_FUNCTIONAL_STD_VECTOR_SUPPORT
#include <boost/accumulators/numeric/functional/vector.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/covariance.hpp>
#include <boost/accumulators/statistics/variates/covariate.hpp>
#include <boost/math/distributions/normal.hpp>

using namespace boost::accumulators;

//following translated from python
std::vector<double> get_linear_fit(std::vector<double> data, double r2tol, double nSegSize, double start)
{
	std::vector<double> x_data;
	std::vector<double> res;
	std::vector<double> lin_reg_result;

	double slope;
	double intercept;
	double r2;
	auto n_data = static_cast<double>(data.size());
	double i_lin_end;
	double seg_end;

	i_lin_end = 0;

	if (r2tol <= 0.0) {
		r2tol = 0.01;
	}
	else if (r2tol > 1.0) {
		r2tol = 1;
	}

	if (nSegSize > n_data) {
		nSegSize = n_data;
	}

	auto n_seg_num = n_data / nSegSize;

	for (unsigned long i = 0; i < data.size(); i++)
	{
		x_data.push_back(static_cast<double>(i));
	}

	for (long i = 0; i <= n_seg_num; i++)
	{
		i_lin_end += nSegSize;

		seg_end = start + i_lin_end < data.size() ? start + i_lin_end : data.size() - 1;

		lin_reg_result = lin_regress(x_data, data, start, seg_end);

		r2 = lin_reg_result[2];

		if (r2 * r2 >= r2tol) {
			slope = lin_reg_result[0];
			intercept = lin_reg_result[1];
		}

		else if (seg_end == data.size() - 1) {
			break;
		}

		else {
			i_lin_end = i_lin_end != nSegSize ? i_lin_end - nSegSize : 1;
			break;

		}
	}

	res.push_back(i_lin_end);
	res.push_back(slope);
	res.push_back(intercept);

	return res;
}

//following translated from python's scipy library
std::vector<double> lin_regress(std::vector<double> xdata, const std::vector<double>& ydata, double min, double max) 
{
	accumulator_set<double, stats<tag::mean, tag::variance, tag::covariance<double, tag::covariate1> > > x_acc;
	accumulator_set<double, stats<tag::mean, tag::variance, tag::covariance<double, tag::covariate1> > > y_acc;


	for (auto i = static_cast<unsigned long>(min); i <= static_cast<unsigned long>(max); ++i) {
		x_acc(xdata[i-2], covariate1 = ydata[i]);
		y_acc(ydata[i], covariate1 = xdata[i-2]);
	}

	std::vector<double> result;

	auto xmean = mean(x_acc);
	auto ymean = mean(y_acc);

	auto ssxm = variance(x_acc);
	auto ssym = variance(y_acc);

	auto ssxym = covariance(x_acc);	

	double r;
	auto r_num = ssxym;
	auto r_den = sqrt(ssxm * ssym);

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


std::vector<double> gaus_kern(double sigma, int width) 
{

	std::vector<double> kernel;
	auto mean = width / 2.0;
	auto sum = 0.0;
	boost::math::normal normal_function(mean, sigma);
	for (auto x = 0; x < width; ++x)
		{
			auto x_pdf = pdf(normal_function, x);
			kernel.push_back(x_pdf);
			sum += x_pdf;
		}

	for (auto x = 0; x < width; ++x) { kernel[x] /= sum;}

	return kernel;
}

std::vector<double> gaus_blur(std::vector<double> data, int width, double sigma)
{
	auto gk =  gaus_kern(sigma, width);
	std::vector<double> result;

	auto centre_point = static_cast<int>(round(width / 2));

	for (unsigned long i = 0; i < data.size(); ++i) {
		double value = 0;
		for (long ii = 0; ii <= width; ++i) {
			if (i + ii - centre_point < 0) { continue; }
			if (i + ii - centre_point > data.size()) { continue; }
			value += data[ii] * gk[ii];
		}
		result.push_back(value);
	}

	return result;
}

