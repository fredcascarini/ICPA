#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

#include "LinearRegression.h"

#include <iostream>

CoordSet::CoordSet(std::vector<std::string> DataLine, Trajectory& T, SingletonTrajectories& ST)  //constructor
{
	for (unsigned i = 0; i < 3; i++) 
	{
		if      (i == 0) { T.add_coord_set(*this); }
		else if (i == 1) { Atoms = DataLine[i]; index = ST.find_index(Atoms); }
		else             { CreateTrajPoints(DataLine, ST); }
	}
}

size_t CoordSet::add_traj_point(TrajectoryPoint& TrPoint) { list_of_traj_points.push_back(TrPoint);	return list_of_traj_points.size() - 1; }

TrajectoryPoint CoordSet::return_traj_point(int index) { return list_of_traj_points[index]; }

void CoordSet::CreateTrajPoints(std::vector<std::string> Data, SingletonTrajectories& ST) {
	
	std::vector<double> dData;
	
	for (unsigned long i = 2; i < Data.size(); ++i) { dData.push_back(std::stod(Data[i]));}
	double start = 2;
	double length = static_cast<double>(dData.size());
	while (start < length) {
		std::vector<double> EndSlopeIntercept = GetLinearFit(dData, 0.9, 1.0, start);

		double end = (start + EndSlopeIntercept[0] < dData.size()) ? (start + EndSlopeIntercept[0]) : dData.size();
		std::vector<double>::const_iterator start_of_vec = dData.begin();
		std::vector<double> linearData(start_of_vec + start, start_of_vec + end);
		TrajectoryPoint trajP(linearData,*this,ST,EndSlopeIntercept[1],EndSlopeIntercept[2]);
		start = end + 1;
	}
	return;
}
