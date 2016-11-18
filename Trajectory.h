#ifndef STRING_H
#define STRING_H


#include <string>
#include <vector>

class TrajectoryPoint;
class SingletonTrajectories;


class Trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	Trajectory(std::string IMatt, SingletonTrajectories& ST);  //constructor 1


	std::string return_atoms() { return att;} 

	int return_index() { return index; }

	size_t add_traj_point(TrajectoryPoint& TrPoint);

	TrajectoryPoint return_traj_point(int index);

	size_t return_TrajectoriesID();

	~Trajectory() {};

protected:
	std::vector<double> coordinates;
	std::vector<TrajectoryPoint> list_of_trajectory_points;
	int index;
	std::string att;
	size_t TrajectoriesID;
};

#endif