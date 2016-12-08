#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"

Trajectory::Trajectory(SingletonTrajectories* ST, std::vector< std::vector<std::string> > data_lines)  //constructor
{
	number_of_coordinates = data_lines.size();

	//Analysis begins:
	for (auto i = 0; i < number_of_coordinates + 1; i++) {
		if (i != number_of_coordinates) {
			auto C = new CoordSet (data_lines[i], this, ST);
		}
		if (i == number_of_coordinates) {
			auto C2 = new CoordSet (list_of_coord_sets);
		}
	}

	ST->add_trajectory(this); //populate ST array of pointers
}

size_t Trajectory::add_coord_set(CoordSet* CoSet)
{
	list_of_coord_sets.push_back(CoSet);

	return list_of_coord_sets.size() - 1;
}

CoordSet* Trajectory::return_coord_set(int index) {
	return list_of_coord_sets[index];
}

size_t Trajectory::return_TrajectoriesID() const
{
	return trajectoriesID;
}
