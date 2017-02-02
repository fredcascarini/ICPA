#ifndef STRING_H
#define STRING_H

#include <vector>
#include <boost/any.hpp>
#include <functional>

class set_of_trajectories;

class coord_set; 


class trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	trajectory(set_of_trajectories* ST, std::vector<std::vector<std::string>> data_line); //constructor
	~trajectory();

	void analyse();

	void add_coord_set(coord_set* CoSet); //add pointer to coord_set to list_of_coord_sets, returns index of ppinter as size_t
	coord_set* return_coord_set(int index); //returns coord_set instance at location index in list_of_coord_sets as pointer
	void analyse_coord_sets(std::vector<coord_set*> setOfCSInstances) const;
	std::string DetermineTrajType(std::vector<boost::any> traj_details) const;

	//defined as well as declared here so can be used as a static function with trajectory::find_val_loc_by_fn<>()
	template<typename T>
	static std::vector<T> find_val_loc_by_fn(std::vector<T> arr, std::function<bool(T, T)> fn) {
		auto curr_element = *arr.begin();
		auto curr_element_location = 0;
		auto index = 0;

		for (auto it = arr.begin(); it != arr.end(); ++it)
		{
			auto val = *it;
			if (fn(val, curr_element))
			{
				curr_element = val;
				curr_element_location = index;
			}

			index++;
		}

		std::vector<T> results;

		results.push_back(curr_element);
		results.push_back(curr_element_location);

		return results;
	}


private:

	std::vector<coord_set*> list_of_coord_sets; //vector of coord_set instances, each as a pointer
	int number_of_coordinates; //number of coordinate sets in the data set
	set_of_trajectories* st_;
	std::vector<std::vector<std::string>> data_lines_;
};

#endif
