#include "set_of_trajectories.h"
#include "Trajectory.h"
#include "coord_set.h"

using namespace std;

trajectory::trajectory(set_of_trajectories* ST, vector< vector<string> > data_lines)  //constructor
	: st_(ST), data_lines_(data_lines)
{
	number_of_coordinates = data_lines_.size();
}

trajectory::~trajectory()  
{
	for (coord_set* value_ : list_of_coord_sets) {
		delete value_;
	}
}

void trajectory::analyse()
{
	//Analysis begins:
	for (auto i = 0; i < number_of_coordinates; i++) {
		auto C = new coord_set(data_lines_[i], st_);
		C->analyse();
		add_coord_set(C);
	}

	// todo C2 is not a CordSet
	auto C2 = new coord_set(list_of_coord_sets);

	st_->add_trajectory(this); //populate ST array of pointers	
}

void trajectory::add_coord_set(coord_set* CoSet)
{
	list_of_coord_sets.push_back(CoSet);
}

coord_set* trajectory::return_coord_set(int index) {
	return list_of_coord_sets[index];
}

