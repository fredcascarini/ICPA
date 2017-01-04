#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>

class set_of_trajectories;

class coord_set;
class trajectory_point;


class trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	trajectory(set_of_trajectories* ST, std::vector< std::vector<std::string> > data_line);	//constructor
	
	size_t					add_coord_set(coord_set* CoSet);										//add pointer to coord_set to list_of_coord_sets, returns index of ppinter as size_t
	coord_set*				return_coord_set(int index);										//returns coord_set instance at location index in list_of_coord_sets as pointer
	size_t					return_TrajectoriesID() const;											//returns index of this trajectory instance in set_of_trajectories

private:

	std::vector<double>		coordinates;														//vector of Coordinates, each as a double
	std::vector<coord_set*>	list_of_coord_sets;													//vector of coord_set instances, each as a pointer
	size_t					trajectoriesID;														//index of this trajectory instance in set_of_trajectories
	int						number_of_coordinates;												//number of coordinate sets in the data set
	//~trajectory() {};																			//destructor

};

#endif