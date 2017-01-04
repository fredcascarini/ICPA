#ifndef SINGLETONTRAJECTORIES_H
#define SINGLETONTRAJECTORIES_H

#include <vector>
#include <map>

class trajectory;
class coord_set;
class trajectory_point;

class set_of_trajectories																					//Handles stuff that is common amoungs all trajectories
{
public:

	static set_of_trajectories*				instance()														//Returns the singleton instance
	{ static set_of_trajectories S; return &S; }

	std::vector<std::vector<std::string>>	ret_list() const												//returns list_of_bond_types
	{ return list_of_bond_types; }

	std::vector<std::string>				find_bond_type(int index)										//finds type of specified index in list_of_types
	{ return list_of_bond_types[index]; }


	int										find_bond_type_index(std::vector<std::string> type_to_indx);	//finds index of specified type in list_of_bond_types
	double									find_in_lengths(std::string bond) const;						//returns length of bond in dict_of_lengths
	bool									test_bound(std::vector<std::string> bond, double length) const;	//returns whether length is less than or equal to maximum bound length of the bond
	size_t									add_trajectory(trajectory* traj);								//add trajectory pointer to list_of_trajectories
	trajectory*								return_trajectory(int index);									// returns pointer at specified index


private:

	set_of_trajectories();																			//Generates singleton instance
	~set_of_trajectories();																			//Destructor

	static std::map<std::string, double>		create_map();										//Generates dict_of_length
	void										add_bond_type(std::vector<std::string> type);		//adds type to list_of_types

	std::map<std::string, double>				dict_of_lengths;									//dict of bond lengths
	std::vector<std::vector<std::string>>		list_of_bond_types;									//list of bond types
	std::vector<trajectory*>					list_of_trajectories;								//list of trajectory class pointers
	std::vector<std::string>					list_of_traj_types_str;								//list of trajectory point types

};


#endif