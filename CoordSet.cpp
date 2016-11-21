#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

CoordSet::CoordSet(std::string IMatt, Trajectory& T, SingletonTrajectories& ST)  //constructor
{
	Atoms = IMatt; //Bond type
	index = ST.find_index(Atoms);
	T.add_coord_set(*this);
}

size_t CoordSet::add_traj_point(TrajectoryPoint& TrPoint)
{
	list_of_traj_points.push_back(TrPoint);

	return list_of_traj_points.size() - 1;
}

TrajectoryPoint CoordSet::return_traj_point(int index) {
	return list_of_traj_points[index];
}
