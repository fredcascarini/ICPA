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

	double find_in_lengths(std::string bond) const //returns length of bond in dict_of_lengths
	{
		return dict_of_lengths.find(bond)->second;
	}

	std::vector<std::string> ret_list() const  //returns list_of_types
	{
		return list_of_types;
	}

	int find_index(std::string type_to_indx) //finds index of specified type in list_of_types
	{
		std::vector<std::string>::iterator loc = find(list_of_types.begin(), list_of_types.end(), type_to_indx);
		if (loc == list_of_types.end()) {
			add_type(type_to_indx);
			find_index(type_to_indx);
		}
		return distance(list_of_types.begin(), loc);
	}

	bool test_bound(std::string bond, double length) const //returns whether length is less than or equal to maximum bound length of the bond
	{
		double bound_length = find_in_lengths(bond);
		return bound_length >= length;
	}


private:
	SingletonTrajectories() { dict_of_lengths = create_map(); } //Generates singleton instance

	std::map<std::string, double> create_map() //Generates dict_of_length
	{
		std::map<std::string, double> m;
		m["Cl-H"] = 2.2;
		return m;
	}

	void add_type(std::string type) //adds type to list_of_types
	{
		list_of_types.push_back(type);
	}

	std::map<std::string, double> dict_of_lengths;
	std::vector<std::string>      list_of_types;
	std::vector<int>         list_of_trajectories;

	~SingletonTrajectories() {}; //destructor

};