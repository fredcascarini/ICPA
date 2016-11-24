#ifndef COORDSET_H
#define COORDSET_H

#include <string>
#include <vector>

class SingletonTrajectories;
class Trajectory;

class TrajectoryPoint;

class CoordSet {

public:

	CoordSet(std::vector<std::string> DataLine, Trajectory& T, SingletonTrajectories& ST);

	std::string return_atoms() { return Atoms; }

	size_t add_traj_point(TrajectoryPoint& TrPoint);

	int return_index() { return index; }
	std::string return_type() { return Type; }

	TrajectoryPoint return_traj_point(int index);

	void CreateTrajPoints(std::vector<std::string> Data, SingletonTrajectories& ST);


private:
	std::string Atoms;
	int index;
	std::string Type;
	std::vector<TrajectoryPoint> list_of_traj_points;
};

#endif

