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

	TrajectoryPoint(std::vector<double> IMcot, CoordSet& CS, SingletonTrajectories& ST, double INslope, double INintercept);	//constructor

	std::vector<double>		return_coordinate()																					//return set of Coordinates that this TrajectoryPoint instance covers
	{ return Coordinates; }

	bool					bound(const SingletonTrajectories& ST, CoordSet& CS);												//returns whether the coordinate is bound at this period of the trajectory as boolean


protected:

	std::vector<double>		Coordinates;																						//Vector of coordinates as doubles									
	int						index;																								//index of this instance in CoordSet
	bool					isBound;																							//whether the coordinates is bound as boolean
	double					slope;																								//Slope of LinReg line
	double					intercept;																							//intercept of LinReg line
};

#endif