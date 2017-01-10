#include "set_of_trajectories.h"
#include "Trajectory.h"

double set_of_trajectories::find_in_lengths(std::string bond) const //returns length of bond in dict_of_lengths
{ return dict_of_lengths.find(bond)->second; }

int set_of_trajectories::find_bond_type_index(std::vector<std::string> type_to_indx) //finds index of specified type in list_of_types
{
	std::vector<std::vector<std::string>>::iterator loc;
	loc = find(list_of_bond_types.begin(), list_of_bond_types.end(), type_to_indx);
	if (loc == list_of_bond_types.end())
	{
		add_bond_type(type_to_indx);
		return find_bond_type_index(type_to_indx);
	}
	return distance(list_of_bond_types.begin(), loc);
}

bool set_of_trajectories::test_bound(std::string bond_as_string, double length) const //returns whether length is less than or equal to maximum bound length of the bond
{
	auto bound_length = find_in_lengths(bond_as_string);
	return bound_length >= abs(length);
}

std::map<std::string, double> set_of_trajectories::create_map() //Generates dict_of_length
{
	std::map<std::string, double> m;
	m["CL_HM1"] = 1.5;
	m["CL_HM2"] = 1.5;
	m["CL_HM3"] = 1.5;
	m["CL_C1"] = 2.2;
	m["CL_C2"] = 2.2;

	return m;
}

void set_of_trajectories::add_bond_type(std::vector<std::string> type) //adds type to list_of_types
{ list_of_bond_types.push_back(type); }

set_of_trajectories::set_of_trajectories() { dict_of_lengths = create_map(); }

size_t set_of_trajectories::add_trajectory(trajectory* traj)
{
	list_of_trajectories.push_back(traj);
	return list_of_trajectories.size() - 1;
}

trajectory* set_of_trajectories::return_trajectory(int index) { return list_of_trajectories[index]; }
