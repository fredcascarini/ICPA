#ifndef STRING_H
#define STRING_H

#include <vector>
#include <boost/any.hpp>

class set_of_trajectories;

class coord_set;
class TrajectoryPoint;


class trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	trajectory(set_of_trajectories* ST, std::vector<std::vector<std::string>> data_line); //constructor
	~trajectory();

	void analyse();

	void add_coord_set(coord_set* CoSet); //add pointer to coord_set to list_of_coord_sets, returns index of ppinter as size_t
	coord_set* return_coord_set(int index); //returns coord_set instance at location index in list_of_coord_sets as pointer
	void analyse_coord_sets(std::vector<coord_set*> setOfCSInstances);
	static std::vector<int> find_min_val_loc(std::vector<int> arr);
	std::string DetermineTrajType(std::vector<boost::any> traj_details) const;

private:

	std::vector<coord_set*> list_of_coord_sets; //vector of coord_set instances, each as a pointer
	int number_of_coordinates; //number of coordinate sets in the data set
	set_of_trajectories* st_;
	std::vector<std::vector<std::string>> data_lines_;
};

#endif
