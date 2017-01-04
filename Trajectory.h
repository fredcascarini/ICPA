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

	Trajectory(SingletonTrajectories* ST, std::vector< std::vector<std::string> > data_line);	//constructor
	~Trajectory();
	void analyse();

	void					add_coord_set(CoordSet* CoSet);										//add pointer to CoordSet to list_of_coord_sets, returns index of ppinter as size_t
	CoordSet*				return_coord_set(int index);										//returns CoordSet instance at location index in list_of_coord_sets as pointer

private:

	std::vector<CoordSet*>	list_of_coord_sets;													//vector of CoordSet instances, each as a pointer
	int						number_of_coordinates;												//number of coordinate sets in the data set
	SingletonTrajectories* st_;
	std::vector<std::vector<std::string>> data_lines_;

};

#endif