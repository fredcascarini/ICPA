#ifndef COORDSET_H
#define COORDSET_H

#include <vector>
#include "set_of_trajectories.h"

class set_of_trajectories;
class trajectory;
class trajectory_point;

enum class dataType
{
	length,
	angle,
	dihedral,
	error
};

class coord_set
{
public:

	//constructor
	coord_set(std::vector<std::string> DataLine, set_of_trajectories* ST);

	//simple accessor functions
	std::vector<trajectory_point*>	return_tP_list()	const { return list_of_traj_points; }	//return list of pointers to trajpoint instances
	std::vector<int>				return_tP_loc()		const { return location_of_traj_points; }	//return list of pointers to trajpoint instances
	std::vector<std::string>		return_atoms()		const { return atoms; }					//return atoms relevant to this coord_set as vector of strings
	std::string						return_name()		const { return name; }					//return atoms relevant to this coord_set as a string
	dataType						return_type()		const { return type; }					//return coordinate type as string
	int								return_index()		const { return index; }					//return bond type index from ST as int
	int								return_num_dp()		const { return number_of_data_points; }	//return number of individual data points in this coord set

	//other functions
	std::vector<std::string>	SplitAtoms(std::string atoms);												//Split list of atoms into vector array of atom names
	size_t						add_traj_point(trajectory_point* TrPoint);									//adds TrajectoryPoint instance to list_of_traj_points. Returns index of new pointer as size_t
	trajectory_point*			return_traj_point(int index);												//returns TrajectoryPoint instance at index in list_of_traj_points as pointer
	void						CreateTrajPoints(std::vector<std::string> Data, set_of_trajectories* ST);	//Separates data into a string of TrajectoryPoints
	void						analyse();


	//moved
	//std::string DetermineTrajType(std::vector<boost::any> traj_details) const; //Determines trajectory type, returns type as string
	//static inline std::vector<int> find_min_val_loc(std::vector<int> arr); //finds minimum value in an array of ints, returns vector - vector[0] is min value, vector[1] is location


private:

	//returned via simple accessor functions
	std::vector<trajectory_point*> list_of_traj_points; //vector of pointers to TrajectoryPoint instances
	std::vector<std::string> atoms;						//string of atoms that co-ordinate refers to
	std::string name;									//atoms as a single string
	dataType type;										//coordinate type: length, angle, dihedral etc.
	int index;											//bond type index from SingletonTrajectories
	int number_of_data_points;							//number of individual data points in coord set

	//other variables
	std::vector<int> location_of_traj_points;			//vector of start indices of each traj point
	std::vector<int> set_of_traj_types;					//vector of trajectory types (e.g. Roaming, C1 complex) as numbers

	const int num_header_cols = 2;

	std::vector<std::string> data_line_;
	set_of_trajectories* st_;
};

#endif
