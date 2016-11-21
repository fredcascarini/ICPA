#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

TrajectoryPoint::TrajectoryPoint(double IMcot, CoordSet& CS, SingletonTrajectories& ST)
{
	Coordinate = IMcot;
	index = CS.return_index();
	CS.add_traj_point(*this);

	isBound = (CS.return_type() == "length") ? bound(ST, CS) : false;
}

bool TrajectoryPoint::bound(const SingletonTrajectories& ST, CoordSet& CS) {
	return ST.test_bound(CS.return_atoms(), Coordinate);
}
