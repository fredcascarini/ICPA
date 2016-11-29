#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

#include "LinearRegression.h"

#include <iostream>

CoordSet::CoordSet(std::vector<std::string> DataLine, Trajectory& T, SingletonTrajectories& ST)  //constructor for data coordinate sets
{
	number_of_data_points = DataLine.size() - 2.0;
	for (unsigned i = 0; i < 3; i++) 
	{
		if      (i == 0) { T.add_coord_set(*this); }
		else if (i == 1) { atoms = DataLine[i]; index = ST.find_bond_type_index(atoms); }
		else             { CreateTrajPoints(DataLine, ST); }
	}
}

CoordSet::CoordSet(std::vector<CoordSet> setOfCSInstances) //constructor for traj type sets
{
	double size = setOfCSInstances[0].number_of_data_points;
	for (double i = 0; i < size; ++i) {

	}
}

size_t CoordSet::add_traj_point(TrajectoryPoint& TrPoint) { list_of_traj_points.push_back(TrPoint);	return list_of_traj_points.size() - 1; }

std::string CoordSet::DetermineTrajType(std::vector<std::string> atom_types, std::vector<std::string> coordinates) 
{
	return "Unimplemented";
}

TrajectoryPoint CoordSet::return_traj_point(int index) { return list_of_traj_points[index]; }

void CoordSet::CreateTrajPoints(std::vector<std::string> Data, SingletonTrajectories& ST) {
	
	std::vector<double> dData;
	
	for (unsigned long i = 2; i < Data.size(); ++i) { dData.push_back(std::stod(Data[i]));}
	double start = 2;
	while (start < number_of_data_points) {
		std::vector<double> EndSlopeIntercept = GetLinearFit(dData, 0.9, 1.0, start);

		double end = (start + EndSlopeIntercept[0] < dData.size()) ? (start + EndSlopeIntercept[0]) : dData.size();
		std::vector<double>::const_iterator start_of_vec = dData.begin();
		std::vector<double> linearData(start_of_vec + start, start_of_vec + end);
		TrajectoryPoint trajP(linearData,*this,ST,EndSlopeIntercept[1],EndSlopeIntercept[2]);
		start = end + 1;
	}
	return;
}
