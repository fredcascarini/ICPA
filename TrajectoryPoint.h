#ifndef TRAJECTORYPOINT_H
#define TRAJECTORYPOINT_H

#include <string>
#include <vector>

class SingletonTrajectories;
class Trajectory;
class CoordSet;

class TrajectoryPoint//handles stuff that is specific to one data point
{

public:

	TrajectoryPoint(std::vector<double> IMcot, CoordSet& CS, SingletonTrajectories& ST, double INslope, double INintercept);

	std::vector<double> return_coordinate() { return Coordinates; }

	bool bound(const SingletonTrajectories& ST, CoordSet& CS);

	~TrajectoryPoint() {};

protected:
	std::vector<double> Coordinates;
	int index;
	bool isBound;
	double slope;
	double intercept;
};

#endif