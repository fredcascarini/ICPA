#include <string>


class SingletonTrajectories;
class TrajectoryPoint;

class Trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	Trajectory(std::string IMatt, SingletonTrajectories& ST)  //constructor
	{
		att = IMatt; //Bond type
		index = ST.find_index(att);
	}

	Trajectory(const Trajectory &traj, SingletonTrajectories& ST)
	{
		att = traj.att;
		index = ST.find_index(att);
	}

	std::string return_atoms() {
		return att;
	}

	int return_index() {
		return index;
	}

	void add_traj_point(TrajectoryPoint& TrPoint)
	{
		TrajectoryPoints.push_back(TrPoint);
	}


protected:
	std::vector<double> coordinates;
	std::vector<TrajectoryPoint&> TrajectoryPoints;
	int index;
	std::string att;
	int TrajectoriesID;
};