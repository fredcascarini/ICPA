#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "TrajectoryPoint.h"

TrajectoryPoint::TrajectoryPoint(double IMcot, Trajectory& traj)
{
	cot = IMcot;
	att = traj.return_atoms();
	index = traj.return_index();
	traj.add_traj_point(*this);
}


bool TrajectoryPoint::bound(const SingletonTrajectories& ST, Trajectory& traj) {
	return ST.test_bound(traj.return_atoms(), cot);
}