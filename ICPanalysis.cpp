#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <boost/tokenizer.hpp>

#include <ctime>

#include "set_of_trajectories.h"
#include "trajectory.h"
#include "CoordSet.h"

using namespace std;
using namespace boost;

set_of_trajectories* s = set_of_trajectories::instance();

vector<string> StringSplit(string s, string delimiter) {

	size_t pos;

	string token;
	
	vector<string> split_string;

	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		split_string.push_back(token);
		s = s.substr(pos + delimiter.length());
	}

	return split_string;
}


int main() {

	clock_t start;
	start = clock();
	double duration;
	double duration2;

	duration = clock() / static_cast<double>(CLOCKS_PER_SEC);

	string data("file_icp.dat");

	ifstream input(data.c_str());
	if (!input.is_open()) { cout << "Error in opening input file"; return 1; } 

	char_separator<char> sep(" ");
	typedef tokenizer< char_separator<char> > Tokenizer;

	vector < vector <string> > vec_traj;
	vector <string> vec;
	string line;
	double last_traj_no;
	bool new_traj;

	new_traj = true;

	auto tracker = 0;

	while (getline(input, line))
	{
		auto print_on = 50;

		if (tracker % print_on == 0) { 
			duration2 = (clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
			cout << "\n\n" << tracker/print_on << "\t" << duration2 - duration << "\n\n"; 
			duration = duration2;
		}

		++tracker;

		Tokenizer tok(line, sep);
		vec.assign(tok.begin(), tok.end());
		if (new_traj) {
			cout << "\n\n";
			vec_traj.push_back(vec);
			last_traj_no = stod(vec[0]);
			new_traj = false;
			continue;
		}
		if (stod(vec[0]) == last_traj_no) {
			vec_traj.push_back(vec);
			continue;
		}
		auto traj = new trajectory (s,vec_traj);
		vec_traj.clear();
		vec_traj.push_back(vec);
		last_traj_no = stod(vec[0]);
	}


	cout << (clock() - start) / static_cast<double>(CLOCKS_PER_SEC);

	

    return 0;
}