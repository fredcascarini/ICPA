#include "set_of_trajectories.h"
#include "trajectory.h"
#include "coord_set.h"
#include "trajectory_point.h"

trajectory_point::trajectory_point(std::vector<double> linCot, std::vector<double> trajCot, coord_set* CS, set_of_trajectories* ST, double INslope, double INintercept)
{
	LinCoordinates = linCot;
	TrajCoordinates = trajCot;

	index = CS->return_index();
	slope = INslope;
	intercept = INintercept;

	isBound = CS->return_type() == dataType::length ? bound(ST, CS) : false;

	CS->add_traj_point(this);
}

bool trajectory_point::bound(const set_of_trajectories* ST, coord_set* CS) const { return ST->test_bound(CS->return_name(), trajectory::find_val_loc_by_fn<double>(LinCoordinates, std::greater<double>())[0]); }
