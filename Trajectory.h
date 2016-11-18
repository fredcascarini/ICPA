#ifndef STRING_H
#define STRING_H


#include <string>


class SingletonTrajectories;
class TrajectoryPoint;

class Trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	Trajectory(std::string IMatt, SingletonTrajectories& ST);  //constructor 1

	Trajectory(const Trajectory &traj, SingletonTrajectories& ST); //constructor 2


	std::string return_atoms() { return att;} 

	int return_index() { return index; }

	void add_traj_point(TrajectoryPoint& TrPoint);


protected:
	std::vector<double> coordinates;
	std::vector<TrajectoryPoint&> TrajectoryPoints;
	int index;
	std::string att;
	int TrajectoriesID;
};

#endif