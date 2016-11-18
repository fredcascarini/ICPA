#include "SingletonTrajectories.h"

double SingletonTrajectories::find_in_lengths(std::string bond) const //returns length of bond in dict_of_lengths
{
	return dict_of_lengths.find(bond)->second;
}

int SingletonTrajectories::find_index(std::string type_to_indx) //finds index of specified type in list_of_types
{
	std::vector<std::string>::iterator loc = find(list_of_types.begin(), list_of_types.end(), type_to_indx);
	if (loc == list_of_types.end()) {
		add_type(type_to_indx);
		find_index(type_to_indx);
	}
	return distance(list_of_types.begin(), loc);
}

bool SingletonTrajectories::test_bound(std::string bond, double length) const //returns whether length is less than or equal to maximum bound length of the bond
{
	double bound_length = find_in_lengths(bond);
	return bound_length >= length;
}

std::map<std::string, double> SingletonTrajectories::create_map() //Generates dict_of_length
{
	std::map<std::string, double> m;
	m["Cl-H"] = 2.2;
	return m;
}

void SingletonTrajectories::add_type(std::string type) //adds type to list_of_types
{
	list_of_types.push_back(type);
}