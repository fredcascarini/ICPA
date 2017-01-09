#include "LinearRegression.h"
#include <cmath>
#include <vector>
#include <numeric>

//#define BOOST_NUMERIC_FUNCTIONAL_STD_VECTOR_SUPPORT
//#include <boost/accumulators/numeric/functional/vector.hpp>
//#include <boost/accumulators/accumulators.hpp>
//#include <boost/accumulators/statistics/stats.hpp>
//#include <boost/accumulators/statistics/variance.hpp>
//#include <boost/accumulators/statistics/covariance.hpp>
//#include <boost/accumulators/statistics/variates/covariate.hpp>
#include <boost/math/distributions/normal.hpp>

//using namespace boost::accumulators;

//following translated from python
std::vector<double> get_linear_fit(std::vector<double> data, double r2tol, int nSegSize, int start)
{
	std::vector<double> x_data;
	std::vector<double> res;
	std::vector<double> lin_reg_result;

	double slope;
	double intercept;
	double r;
	auto n_data = static_cast<int>(data.size());
	int i_lin_end = 0;
	int seg_end;

	//fix erroneous inputs
	if		(r2tol <= 0.0)		{ r2tol = 0.01; }
	else if (r2tol > 1.0)		{ r2tol = 1; }
	if		(nSegSize > n_data) { nSegSize = n_data;}

	auto n_seg_num = n_data / nSegSize;

	//populate x_data with sequence of integers as long as vector data
	for (unsigned long i = 0; i < data.size(); i++) { x_data.push_back(static_cast<double>(i)); }


	for (long i = 0; i <= n_seg_num; i++)
	{
		i_lin_end += nSegSize;

		if		(start + i_lin_end < n_data)	{ seg_end = start + i_lin_end;}
		else									{ seg_end = data.size() - 1; }

		lin_reg_result = lin_regress(x_data, data, start, seg_end);

		r = lin_reg_result[2];

		//slope and intercept hold the values for the last fit that fulfils the r2tol requirement
		if (r * r >= r2tol) {
			slope = lin_reg_result[0];
			intercept = lin_reg_result[1];
		}

		else if (seg_end == data.size() - 1) {break;}

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
//returns [slope,intercept,r]
std::vector<double> lin_regress(std::vector<double> xdata, const std::vector<double>& ydata, int min, int max) 
{
	std::vector<double> result;

// alternate version using boost accumulators:
/*	
	accumulator_set<double, stats<tag::mean, tag::variance, tag::covariance<double, tag::covariate1> > > x_acc;
	accumulator_set<double, stats<tag::mean, tag::variance, tag::covariance<double, tag::covariate1> > > y_acc;
	for (auto i = static_cast<unsigned long>(min); i <= static_cast<unsigned long>(max); ++i) {
		x_acc(xdata[i-2], covariate1 = ydata[i]);
		y_acc(ydata[i], covariate1 = xdata[i-2]);
	}
	auto xmean = mean(x_acc);
	auto ymean = mean(y_acc);
	auto ssxm = variance(x_acc);
	auto ssym = variance(y_acc);
	auto ssxym = covariance(x_acc);	
	
*/

	std::vector<double> mvc_xy_res;
	
	mvc_xy_res = mvc_xy(xdata, ydata, min, max);

	double r;
	auto r_num = mvc_xy_res[4];
	auto r_den = sqrt(mvc_xy_res[2] * mvc_xy_res[3]);

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
	
	slope = r_num / mvc_xy_res[2];
	intercept = mvc_xy_res[1] - slope*mvc_xy_res[0];

	result.push_back(slope);
	result.push_back(intercept);
	result.push_back(r);

	return result;
}

std::vector<double> mvc_xy (std::vector<double> xdata, std::vector<double> ydata, int start, int end)
{
	std::vector<double> result;

	double sumx = std::accumulate(&xdata[start], &xdata[end], 0.0);
	double sumy = std::accumulate(&ydata[start], &ydata[end], 0.0);

	double mx = sumx / xdata.size();
	double my = sumy / ydata.size();

	double accumx = 0.0;
	double accumy = 0.0;

	std::for_each(&xdata[start], &xdata[end], [&](const double dx)
	{
		accumx += (dx - mx) * (dx - mx);
	});

	std::for_each(&ydata[start], &ydata[end], [&](const double dy)
	{
		accumy += (dy - my) * (dy - my);
	});

	double varx = accumx / (xdata.size() - 1);
	double vary = accumy / (ydata.size() - 1);

	double accumCovar = 0.0;

	for (int i = 0; i < static_cast<int>(xdata.size()); ++i)
	{
		accumCovar += (xdata[i] - mx) * (ydata[i] - my);
	}

	double covar = accumCovar / (xdata.size() - 1);

	result.push_back(mx);
	result.push_back(my);
	result.push_back(varx);
	result.push_back(vary);
	result.push_back(covar);

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
		for (long ii = 0; ii <= width; ++ii) {
			if (i + ii - centre_point < 0) { continue; }
			if (i + ii - centre_point > data.size()) { continue; }
			value += data[ii] * gk[ii];
		}
		result.push_back(value);
	}

	return result;
}

