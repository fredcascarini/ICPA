#include "set_of_trajectories.h"
#include "Trajectory.h"
#include "coord_set.h"
#include "trajectory_point.h"

#include <algorithm>

trajectory_point::trajectory_point(std::vector<double> IMcot, coord_set* CS, set_of_trajectories* ST, double INslope, double INintercept)
{
	Coordinates = IMcot;
	index = CS->return_index();

	slope = INslope;
	intercept = INintercept;

	isBound = CS->return_type() == dataType::length ? bound(ST, CS) : false;

	CS->add_traj_point(this);
}

bool trajectory_point::bound(const set_of_trajectories* ST, coord_set* CS) { return ST->test_bound(CS->return_name(), *max_element(Coordinates.begin(), Coordinates.end())); }
