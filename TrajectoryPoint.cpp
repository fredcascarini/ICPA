#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"
#include "LinearRegression.h"

#include <algorithm>
#include <iostream>

TrajectoryPoint::TrajectoryPoint(std::vector<double> IMcot, CoordSet& CS, SingletonTrajectories& ST, double INslope, double INintercept)
{
	Coordinates = IMcot;
	CS.add_traj_point(*this);
	index = CS.return_index();

	slope = INslope;
	intercept = INintercept;

	//std::cout << slope << "\t" << intercept << "\t" << Coordinates.size() << "\n";

	isBound = (CS.return_type() == "length") ? bound(ST, CS) : false;
}

bool TrajectoryPoint::bound(const SingletonTrajectories& ST, CoordSet& CS) {
	return ST.test_bound(CS.return_atoms(), *std::max_element(Coordinates.begin(), Coordinates.end()) );
}
