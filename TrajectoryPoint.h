#ifndef TRAJECTORYPOINT_H
#define TRAJECTORYPOINT_H


#include <string>

class TrajectoryPoint//handles stuff that is specific to one data point
{

public:

	TrajectoryPoint(double IMcot, Trajectory& traj);

	double return_coordinate() { return cot; }

	bool bound(const SingletonTrajectories& ST, Trajectory& traj);


protected:
	double cot;
	int TrajectoryID;
	std::string att;
	int index;
};

#endif