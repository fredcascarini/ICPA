#include <string>

class SingletonTrajectories;
class Trajectory;

class TrajectoryPoint//handles stuff that is specific to one data point
{

public:

	TrajectoryPoint(double IMcot, Trajectory& traj)
	{
		cot = IMcot;
		att = traj.return_atoms();
		index = traj.return_index();
		traj.add_traj_point(*this);
	}

	double return_coordinate() {
		return cot;
	}

	bool bound(const SingletonTrajectories& ST, Trajectory& traj) {
		return ST.test_bound(traj.return_atoms(), cot);
	}


protected:
	double cot;
	int TrajectoryID;
	std::string att;
	int index;
};