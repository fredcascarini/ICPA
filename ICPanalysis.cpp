#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <boost\tokenizer.hpp>

using namespace std;
using namespace boost;

class SingletonTrajectories //Handles stuff that is common amoungs all trajectories
{
public:

	static SingletonTrajectories& Instance() {
		static SingletonTrajectories S;
		return S;
	}

	int list;

	double find_in_lengths(string bond) const {
		return dict_of_lengths.find(bond)->second;
	}

	vector<string> ret_list() const {
		return list_of_types;
	}

	void add_type(string type) {
		list_of_types.push_back(type);
	}

	int find_index(string type_to_indx) {
		vector<string>::iterator loc = find(list_of_types.begin(), list_of_types.end(), type_to_indx);
		return distance(list_of_types.begin(), loc);
	}

	bool test_bound(string bond, double length) const {
		double bound_length = find_in_lengths(bond);
		return bound_length >= length;
	}


private:
	SingletonTrajectories() { dict_of_lengths = create_map(); }
	map<string, double> create_map() {
		map<string, double> m;
		m["Cl-H"] = 2.2;
		return m;
	}

	map<string, double> dict_of_lengths;
	vector<string>      list_of_types;
	vector<int>         list_of_trajectories;
	~SingletonTrajectories() {};

};

class Trajectory //handles stuff that is common to an entire line in the icp file
{
public:

	Trajectory(string IMatt, SingletonTrajectories& ST) 
	{
		att = IMatt;
		ST.add_type(att);
		Trajectory::index = ST.find_index(att);
	}

	string return_atoms() {
		return att;
	}

	int return_index() {
		return index;
	}


protected:
	vector<double> coordinates;
	int index;
	string att;
	int TrajectoriesID;
};



class TrajectoryPoint : public Trajectory //handles stuff that is specific to one data point
{

public:

	TrajectoryPoint(double IMcot)
	{
		cot = IMcot;
	}

	double return_coordinate() {
		return cot;
	}

	bool bound(const SingletonTrajectories& ST) {
		return ST.test_bound(Trajectory::att, cot);
	}


protected:
		double cot;
		int TrajectoryID;
};


SingletonTrajectories& s = SingletonTrajectories::Instance();


vector<string> StringSplit(string s, string delimiter) {

	size_t pos = 0;

	string token;
	
	vector<string> SplitString;

	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		SplitString.push_back(token);
		s = s.substr(pos + delimiter.length());
	}

	return SplitString;
}



int main() {

	string data("file_icp.dat");

	ifstream input(data.c_str());
	if (!input.is_open()) return 1;

	char_separator<char> sep(" ");
	typedef tokenizer< char_separator<char> > Tokenizer;

	vector <string> vec;
	string line;

	while (getline(input, line))
	{
		Tokenizer tok(line, sep);
		vec.assign(tok.begin(), tok.end());

	}

	TrajectoryPoint Traj(2.0);


    return 0;
}