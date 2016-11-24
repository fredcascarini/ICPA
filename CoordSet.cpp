#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"
#include <iostream>

CoordSet::CoordSet(std::vector<std::string> DataLine, Trajectory& T, SingletonTrajectories& ST)  //constructor
{
	for (unsigned i = 0; i < DataLine.size(); i++) {
		if      (i == 0) { T.add_coord_set(*this); }
		else if (i == 1) { Atoms = DataLine[i]; index = ST.find_index(Atoms); std::cout << ST.find_type(index) << "\n"; }
		else             { TrajectoryPoint TP(std::stod(DataLine[i]),*this,ST); }
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
