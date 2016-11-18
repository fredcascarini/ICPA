#ifndef SINGLETONTRAJECTORIES_H
#define SINGLETONTRAJECTORIES_H


#include <vector>
#include <string>
#include <map>

class Trajectory;

class SingletonTrajectories //Handles stuff that is common amoungs all trajectories
{
public:

	static SingletonTrajectories& Instance() //Returns the singleton instance
	{
		static SingletonTrajectories S;
		return S;
	}

	double find_in_lengths(std::string bond) const; //returns length of bond in dict_of_lengths

	std::vector<std::string> ret_list() const { return list_of_types; }  //returns list_of_types

	int find_index(std::string type_to_indx); //finds index of specified type in list_of_types

	bool test_bound(std::string bond, double length) const; //returns whether length is less than or equal to maximum bound length of the bond

	size_t add_trajectory(Trajectory& traj); //add trajectory pointer to list_of_trajectories

	Trajectory return_trajectory(int index);


private:
	SingletonTrajectories(); //Generates singleton instance

	std::map<std::string, double> create_map(); //Generates dict_of_length

	void add_type(std::string type); //adds type to list_of_types

	std::map<std::string, double> dict_of_lengths;
	std::vector<std::string>      list_of_types;
	std::vector<Trajectory>       list_of_trajectories;

	~SingletonTrajectories() {}; //destructor

};


#endif