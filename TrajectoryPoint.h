#ifndef TRAJECTORYPOINT_H
#define TRAJECTORYPOINT_H

#include <vector>

class SingletonTrajectories;
class Trajectory;
class CoordSet;

class TrajectoryPoint//handles stuff that is specific to one data point
{

public:

	TrajectoryPoint(std::vector<double> IMcot, CoordSet& CS, SingletonTrajectories& ST, double INslope, double INintercept);	//constructor

	bool					bound(const SingletonTrajectories& ST, CoordSet& CS);												//returns whether the coordinate is bound at this period of the trajectory as boolean

	double					return_slope() const { return slope; }															//accessor functions
	double					return_intercept() const { return intercept; }
	std::vector<double>		return_coordinate() const { return Coordinates; }
	bool					return_bound() const { return isBound; }




protected:

	std::vector<double>		Coordinates;																						//Vector of coordinates as doubles									
	int						index;																								//index of this instance in CoordSet
	bool					isBound;																							//whether the coordinates is bound as boolean
	double					slope;																								//Slope of LinReg line
	double					intercept;																							//intercept of LinReg line
	//~TrajectoryPoint() {};
};

#endif