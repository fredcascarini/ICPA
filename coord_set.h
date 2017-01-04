#ifndef COORDSET_H
#define COORDSET_H

#include <vector>
#include <boost/any.hpp>

class set_of_trajectories;
class trajectory;
class trajectory_point;

class coord_set {

public:

	coord_set(std::vector<std::string> data_line, trajectory* trj, set_of_trajectories* s_o_t);						//data set constructor
	explicit coord_set(std::vector<coord_set*> set_of_cs_instances);															//type set constructor

	std::vector<std::string>	return_atoms() const
	//return atoms relevant to this coord_set as string
	{ return atoms; }

	int							return_index() const
	//return bond type index from ST as int
	{ return index; }

	std::string					return_type() const
	//return coordinate type as string
	{ return type; }

	static std::vector<std::string>	split_atoms(std::string atoms);													//Split list of atoms into vector array of atom names


	size_t						add_traj_point(trajectory_point* TrPoint);										//adds trajectory_point instance to list_of_traj_points. Returns index of new pointer as size_t
	trajectory_point*			return_traj_point(int index);													//returns trajectory_point instance at index in list_of_traj_points as pointer
	void						create_traj_points(std::vector<std::string> Data, set_of_trajectories* ST);		//Separates data into a string of TrajectoryPoints
	std::string					determine_traj_type(std::vector<boost::any> traj_details) const;						//Determines trajectory type, returns type as string
	static inline std::vector<int>		find_min_val_loc(std::vector<int> arr);											//finds minimum value in an array of ints, returns vector - vector[0] is min value, vector[1] is location


private:
	std::vector<std::string>		atoms;																		//string of atoms that co-ordinate refers to
	int								index;																		//bond type index from set_of_trajectories
	std::string						type;																		//coordinate type: length, angle, dihedral etc.
	std::vector<trajectory_point*>	list_of_traj_points;														//vector of pointers to trajectory_point instances
	std::vector<int>				location_of_traj_points;													//vector of start indices of each traj point
	std::vector<int>				set_of_traj_types;															//vector of trajectory types (e.g. Roaming, C1 complex) as numbers
	int								number_of_data_points;														//number of individual data points in coord set
	//~coord_set() {};
};

#endif

