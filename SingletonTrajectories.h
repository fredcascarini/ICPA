#ifndef SINGLETONTRAJECTORIES_H
#define SINGLETONTRAJECTORIES_H


#include <vector>
#include <string>
#include <map>

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


private:
	SingletonTrajectories() { dict_of_lengths = create_map(); } //Generates singleton instance

	std::map<std::string, double> create_map(); //Generates dict_of_length

	void add_type(std::string type); //adds type to list_of_types

	std::map<std::string, double> dict_of_lengths;
	std::vector<std::string>      list_of_types;
	std::vector<int>         list_of_trajectories;

	~SingletonTrajectories() {}; //destructor

};


#endif