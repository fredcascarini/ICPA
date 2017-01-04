#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

#include <algorithm>

TrajectoryPoint::TrajectoryPoint(std::vector<double> IMcot, CoordSet* CS, SingletonTrajectories* ST, double INslope, double INintercept)
{
	Coordinates = IMcot;
	index = CS->return_index();

	slope = INslope;
	intercept = INintercept;
	
	isBound = (CS->return_type() == dataType::length) ? bound(ST, CS) : false;
}

bool TrajectoryPoint::bound(const SingletonTrajectories* ST, CoordSet* CS) {
	return ST->test_bound(CS->return_name(), *std::max_element(Coordinates.begin(), Coordinates.end()) );
}
