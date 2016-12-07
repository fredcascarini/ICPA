#ifndef STRING_H
#define STRING_H

#include <string>
#include <vector>

class SingletonTrajectories;

class CoordSet;
class TrajectoryPoint;


class Trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	Trajectory(SingletonTrajectories& ST, std::vector< std::vector<std::string> > data_line);	//constructor
	
	size_t					add_coord_set(CoordSet& CoSet);										//add pointer to CoordSet to list_of_coord_sets, returns index of ppinter as size_t
	CoordSet				return_coord_set(int index);										//returns CoordSet instance at location index in list_of_coord_sets as pointer
	size_t					return_TrajectoriesID() const;											//returns index of this Trajectory instance in SingletonTrajectories

private:

	std::vector<double>		coordinates;														//vector of Coordinates, each as a double
	std::vector<CoordSet>	list_of_coord_sets;													//vector of CoordSet instances, each as a pointer
	size_t					trajectoriesID;														//index of this Trajectory instance in SingletonTrajectories
	int						number_of_coordinates;												//number of coordinate sets in the data set
	//~Trajectory() {};																			//destructor

};

#endif