#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"

using namespace std;

Trajectory::Trajectory(SingletonTrajectories* ST, vector< vector<string> > data_lines)  //constructor
	: st_(ST), data_lines_(data_lines)
{
	number_of_coordinates = data_lines_.size();
}

Trajectory::~Trajectory()  
{
	for (CoordSet* value_ : list_of_coord_sets) {
		delete value_;
	}
}

void Trajectory::analyse()
{
	//Analysis begins:
	for (auto i = 0; i < number_of_coordinates; i++) {
		auto C = new CoordSet(data_lines_[i], st_);
		C->analyse();
		add_coord_set(C);
	}

	// todo C2 is not a CordSet
	auto C2 = new CoordSet(list_of_coord_sets);

	st_->add_trajectory(this); //populate ST array of pointers	
}

void Trajectory::add_coord_set(CoordSet* CoSet)
{
	list_of_coord_sets.push_back(CoSet);
}

CoordSet* Trajectory::return_coord_set(int index) {
	return list_of_coord_sets[index];
}

