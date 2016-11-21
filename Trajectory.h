#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>

class SingletonTrajectories;

class CoordSet;
class TrajectoryPoint;


class Trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	Trajectory(SingletonTrajectories& ST, std::vector<std::string> data_line);  //constructor

	size_t add_coord_set(CoordSet& TrPoint);

	CoordSet return_coord_set(int index);

	size_t return_TrajectoriesID();

	~Trajectory() {};

private:
	std::vector<double> coordinates;
	std::vector<CoordSet> list_of_coord_sets;
	size_t TrajectoriesID;
};

#endif