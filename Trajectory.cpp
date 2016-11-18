#include "Trajectory.h"

class TrajectoryPoint;
class SingletonTrajectories;

Trajectory::Trajectory(std::string IMatt, SingletonTrajectories& ST)  //constructor
{
	att = IMatt; //Bond type
	index = ST.find_index(att);
}

Trajectory::Trajectory(const Trajectory &traj, SingletonTrajectories& ST)
{
	att = traj.att;
	index = ST.find_index(att);
}

void Trajectory::add_traj_point(TrajectoryPoint& TrPoint)
{
	TrajectoryPoints.push_back(TrPoint);
}