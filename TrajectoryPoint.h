#ifndef TRAJECTORYPOINT_H
#define TRAJECTORYPOINT_H

#include <string>

class SingletonTrajectories;
class Trajectory;
class CoordSet;

class TrajectoryPoint//handles stuff that is specific to one data point
{

public:

	TrajectoryPoint(double IMcot, CoordSet& CS, SingletonTrajectories& ST);

	double return_coordinate() { return Coordinate; }

	bool bound(const SingletonTrajectories& ST, CoordSet& CS);

	~TrajectoryPoint() {};

protected:
	double Coordinate;
	int index;
	bool isBound;
};

#endif