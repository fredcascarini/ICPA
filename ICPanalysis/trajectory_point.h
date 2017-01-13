#ifndef TRAJECTORYPOINT_H
#define TRAJECTORYPOINT_H

#include <vector>

class set_of_trajectories;
class trajectory;
class coord_set;

class trajectory_point//handles stuff that is specific to one data point
{
public:

	trajectory_point(std::vector<double> linCot, std::vector<double> trajCot, coord_set* CS, set_of_trajectories* ST, double INslope, double INintercept); //constructor

	bool bound(const set_of_trajectories* ST, coord_set* CS) const; //returns whether the coordinate is bound at this period of the trajectory as boolean

	//accessor functions
	double return_slope() const { return slope; }
	double return_intercept() const { return intercept; }
	std::vector<double> return_lin_coordinate() const { return LinCoordinates; }
	std::vector<double> return_traj_coordinate() const { return TrajCoordinates; }
	bool return_bound() const { return isBound; }


protected:

	std::vector<double> LinCoordinates; //Vector of approach coordinates as doubles
	std::vector<double> TrajCoordinates; //Vector of the rest of the coordinates as doubles
	int index; //index of this instance in coord_set
	bool isBound; //whether the coordinates is bound as boolean
	double slope; //Slope of LinReg line
	double intercept; //intercept of LinReg line
	//~trajectory_point() {};
};

#endif
