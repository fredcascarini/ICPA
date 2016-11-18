#include "Trajectory.h"
#include "SingletonTrajectories.h"
#include "TrajectoryPoint.h"

Trajectory::Trajectory(std::string IMatt, SingletonTrajectories& ST)  //constructor
{
	att = IMatt; //Bond type
	index = ST.find_index(att);
	ST.add_trajectory(*this);
}

size_t Trajectory::add_traj_point(TrajectoryPoint& TrPoint)
{
	list_of_trajectory_points.push_back(TrPoint);

	return list_of_trajectory_points.size() - 1;
}

TrajectoryPoint Trajectory::return_traj_point(int index) {
	return list_of_trajectory_points[index];
}

size_t Trajectory::return_TrajectoriesID()
{
	return TrajectoriesID;
}
