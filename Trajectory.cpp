#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"
#include "LinearRegression.h"

Trajectory::Trajectory(SingletonTrajectories& ST, std::vector< std::vector<std::string> > data_lines)  //constructor
{
	ST.add_trajectory(*this); //populate ST array of pointers
	number_of_coordinates = data_lines.size();

	//Analysis begins:
	for (int i = 0; i < number_of_coordinates; i++) {
		CoordSet C(data_lines[i], *this, ST);
	}
}

size_t Trajectory::add_coord_set(CoordSet& CoSet)
{
	list_of_coord_sets.push_back(CoSet);

	return list_of_coord_sets.size() - 1;
}

CoordSet Trajectory::return_coord_set(int index) {
	return list_of_coord_sets[index];
}

size_t Trajectory::return_TrajectoriesID()
{
	return trajectoriesID;
}
