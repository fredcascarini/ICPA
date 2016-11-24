#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

#include "LinearRegression.h"

CoordSet::CoordSet(std::vector<std::string> DataLine, Trajectory& T, SingletonTrajectories& ST)  //constructor
{
	for (unsigned i = 0; i < DataLine.size(); i++) {
		if      (i == 0) { T.add_coord_set(*this); }
		else if (i == 1) { Atoms = DataLine[i]; index = ST.find_index(Atoms); }
		else             { CreateTrajPoints(DataLine, ST); }
	}
}

size_t CoordSet::add_traj_point(TrajectoryPoint& TrPoint)
{
	list_of_traj_points.push_back(TrPoint);

	return list_of_traj_points.size() - 1;
}

TrajectoryPoint CoordSet::return_traj_point(int index) {
	return list_of_traj_points[index];
}

void CoordSet::CreateTrajPoints(std::vector<std::string> Data, SingletonTrajectories& ST) {
	
	std::vector<double> dData;
	
	for (unsigned long i = 2; i < Data.size(); ++i) {
		dData.push_back(std::stod(Data[i]));
	}
	unsigned long start = 2;
	while (start < dData.size()) {
		std::vector<double> EndSlopeIntercept = GetLinearFit<double>(dData, 0.9, 10.0, 10.0, start);
		std::vector<double> linearData(&dData[start],&dData[EndSlopeIntercept[0]]);
		TrajectoryPoint trajP(linearData,*this,ST,EndSlopeIntercept[1],EndSlopeIntercept[2]);
	}
	return;
}
