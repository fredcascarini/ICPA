#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <boost\iostreams\stream.hpp>
#include <boost\iostreams\stream_buffer.hpp>
#include <boost\tokenizer.hpp>

using namespace std;
using namespace boost;
namespace io = boost::iostreams;

class Trajectories //Handles stuff that is common amoungs all trajectories
{
public:
	Trajectories() {}
	int list;
	static map<string, double> create_map() {
		map<string, double> m;
		m["Cl-H"] = 2.2;
		return m;
	}

	template <typename T>
	static vector<T> init_list() {
		vector<T> array1;
		array1.reserve(10);
		switch (decltype((t))) {
		case int: 
		case double: 
		case float: 
			array1.push_back(0);
			break;
		case string: 
			array1.push_back("Init");
			break;
		}
		return array1;
	}

	static vector<int> init_list_trajectories() {
		vector<int> array2;
		array2.push_back(0);
		return array2;
	}

	static double find_in_lengths(string bond) {
		return dict_of_lengths.find(bond)->second;
	}

	static vector<string> ret_list() {
		return list_of_types;
	}

	static void add_type(string type) {
		list_of_types.push_back(type);
	}

	static int find_index(string type_to_indx) {
		vector<string>::iterator loc = find(list_of_types.begin(), list_of_types.end(), type_to_indx);
		return distance(list_of_types.begin(), loc);
	}

	static bool test_bound(string bond, double length) {
		double bound_length = find_in_lengths(bond);
		return bound_length >= length;
	}

	int newTrajectories() {

	}

private:
	static const map<string, double> dict_of_lengths;
	static		 vector<string>      list_of_types;
	static       vector<int>         list_of_trajectories;
};

class Trajectory : public Trajectories //handles stuff that is common to an entire line in the icp file
{
public:
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
	TrajectoryPoint(double IMcot, string IMatt);

	double return_coordinate() {
		return cot;
	}

	bool bound() {
		return Trajectories::test_bound(Trajectory::att, cot);
	}


protected:
		double cot;
		int TrajectoryID;
};

TrajectoryPoint::TrajectoryPoint(double IMcot, string IMatt) 
{
	cot = IMcot;
	Trajectory::att = IMatt;
	Trajectories::add_type(att);
	Trajectory::index = find_index(att);
}

const map<string, double> Trajectories::dict_of_lengths = Trajectories::create_map();
vector<string> Trajectories::list_of_types = Trajectories::init_list<string>();
vector<int> Trajectories::list_of_trajectories = Trajectories::init_list<int>();


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
		//cout << "////";
		//for (int i = 0; i < vec.size(); i++) { cout << vec[i] << "|"; }

		//cout << "\n----------------------" << endl; cout.flush();
	}

	TrajectoryPoint Traj(2.2, "Cl-H");

	cout << Traj.return_index();

    return 0;
}