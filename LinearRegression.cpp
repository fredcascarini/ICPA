#include <vector>
#include "boost_1_62_0\boost\accumulators\statistics\covariance.hpp"
#include "boost_1_62_0\boost\accumulators\statistics\variates\covariate.hpp"
#include <cmath>
#include "LinearRegression.h"

using namespace std;
using namespace boost::accumulators;

template <typename T> //following translated from python
std::vector<T> GetLinearFit(const std::vector<T>& data, T r2tol, T nSegSize, T maxSegNum, double start)
{
	std::vector<T> xData;
	std::vector<T> res;
	std::vector<T> linRegResult;

	T slope;
	T intercept;
	T r2;
	T nData = static_cast<T>(data.size());
	long iLinEnd;

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

	T nSegNum = (maxSegNum > (nData / nSegSize)) ? (nData / nSegSize) : maxSegNum;

	for (long i = 0; i < data.size(); i++)
	{
		xData.push_back(static_cast<T>(i));
	}

	for (long i = 0; i <= nSegNum; i++)
	{
		iLinEnd += nSegSize;

		linRegResult = LinRegress(xData, yData, start, iLinEnd);

		r2 = linRegResult[2];

		if (r2 * r2 <= r2tol) {
			slope = linRegResult[0];
			intercept = linRegResult[1]
		}

		else {
			iLinEnd -= nSegSize;
		}
	}

	res.push_back(iLinEnd)
	res.push_back(slope);
	res.push_back(intercept);

	return res;
}

template <typename T> //following translated from python scipy
std::vector<T> LinRegress(const std::vector<T>& xdata, const std::vector<T>& ydata, double min, double max) {
	accumulator_set<T, stats<tag::mean, tag::variance, tag::covariance<T, tag::covariate1> > > x_acc;
	accumulator_set<T, stats<tag::mean, tag::variance, tag::covariance<T, tag::covariate1> > > y_acc;
	
	std::vector<T> xvals = sub(&xdata[min], &xdata[max]);
	std::vector<T> yvals = sub(&ydata[min], &ydata[max]);
	std::vector<T> result;


	T xmean = mean(x_acc);
	T ymean = mean(y_acc);

	T ssxm = variance(x_acc);
	T ssym = variance(y_acc);

	T ssxym = covariance(x_acc);
	T ssyxm = covariance(y_acc);

	T r;
	T r_num = ssxym;
	T r_den = sqrt(ssxm * ssym);

	T slope;
	T intercept;

	x_acc(xvals, covariate1 = yvals);
	y_acc(yvals, covariate1 = xvals);

	if (r_den == 0.0) {
		r = static_cast<T>(0.0);
	}
	else {
		r = r_num / r_den;
		if (r > 1.0) {
			r = static_cast<T>(1.0);
		}
		else if (r < -1.0) {
			r = static_cast<T>(-1.0);
		}
	}
	
	slope = r_num / ssxm;
	intercept = ymean - slope*xmean;

	result.push_back(slope);
	result.push_back(intercept);
	result.push_back(r);

	return result;
}