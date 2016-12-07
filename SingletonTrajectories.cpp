#include "SingletonTrajectories.h"
#include "Trajectory.h"

#include <iostream>

double SingletonTrajectories::find_in_lengths(std::string bond) const //returns length of bond in dict_of_lengths
{	
	return dict_of_lengths.find(bond)->second;
}

int SingletonTrajectories::find_bond_type_index(std::vector<std::string> type_to_indx) //finds index of specified type in list_of_types
{
	std::vector<std::vector<std::string>>::iterator loc;
	loc = find(list_of_bond_types.begin(), list_of_bond_types.end(), type_to_indx);
	if (loc == list_of_bond_types.end()) {
		add_bond_type(type_to_indx);
		return find_bond_type_index(type_to_indx);
	}
	return distance(list_of_bond_types.begin(), loc);
}

bool SingletonTrajectories::test_bound(std::vector<std::string> bond, double length) const //returns whether length is less than or equal to maximum bound length of the bond
{
	auto bond_as_string = bond[0];
	for (auto i = 1; i != bond.size(); ++i) {
		bond_as_string = bond_as_string + "_" + bond[i];
	}
	auto bound_length = find_in_lengths(bond_as_string);

	return bound_length >= std::abs(length);
}

std::map<std::string, double> SingletonTrajectories::create_map() //Generates dict_of_length
{
	std::map<std::string, double> m;
	std::vector<std::string> bond;
	m["CL_HM1"] = 1.5;
	m["CL_HM2"] = 1.5;
	m["CL_HM3"] = 1.5;
	m["CL_C1"] = 2.2; //placeholder - CORRECT THIS
	m["CL_C2"] = 2.2; //placeholder - CORRECT THIS

	return m;
}

void SingletonTrajectories::add_bond_type(std::vector<std::string> type) //adds type to list_of_types
{
	list_of_bond_types.push_back(type);
}

SingletonTrajectories::SingletonTrajectories() {
	dict_of_lengths = create_map(); 
}

size_t SingletonTrajectories::add_trajectory(Trajectory* traj) {
	list_of_trajectories.push_back(traj);
	return list_of_trajectories.size() - 1;
}

Trajectory* SingletonTrajectories::return_trajectory(int index) {
	return list_of_trajectories[index];
}