#include "set_of_trajectories.h"
#include "trajectory.h"
#include "coord_set.h"

trajectory::trajectory(set_of_trajectories* ST, std::vector< std::vector<std::string> > data_lines)  //constructor
{
	number_of_coordinates = data_lines.size();

	//Analysis begins:
	for (auto i = 0; i < number_of_coordinates + 1; i++) {
		if (i != number_of_coordinates) {
			auto c = new coord_set (data_lines[i], this, ST);
		}
		if (i == number_of_coordinates) {
			auto c2 = new coord_set (list_of_coord_sets);
		}
	}

	ST->add_trajectory(this); //populate ST array of pointers
}

size_t trajectory::add_coord_set(coord_set* CoSet)
{
	list_of_coord_sets.push_back(CoSet);

	return list_of_coord_sets.size() - 1;
}

coord_set* trajectory::return_coord_set(int index) {
	return list_of_coord_sets[index];
}

size_t trajectory::return_TrajectoriesID() const
{
	return trajectoriesID;
}
