#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <typeinfo>
#include <boost\tokenizer.hpp>

#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"
#include "LinearRegression.h"

using namespace std;
using namespace boost;

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
	if (!input.is_open()) { cout << "Error in opening input file"; return 1; } 

	char_separator<char> sep(" ");
	typedef tokenizer< char_separator<char> > Tokenizer;

	vector < vector <string> > vecTraj;
	vector <string> vec;
	string line;
	double last_traj_no;
	bool newTraj;

	newTraj = true;

	int tracker = 0;

	while (getline(input, line))
	{

		int print_on = 50;

		if (tracker % print_on == 0) { std::cout << tracker/print_on << "\t"; }

		++tracker;

		Tokenizer tok(line, sep);
		vec.assign(tok.begin(), tok.end());
		if (newTraj) {
			vecTraj.push_back(vec);
			last_traj_no = stod(vec[0]);
			newTraj = false;
			continue;
		}
		if (stod(vec[0]) == last_traj_no) {
			vecTraj.push_back(vec);
			continue;
		}
		Trajectory traj(s,vecTraj);
		vecTraj.clear();
		vecTraj.push_back(vec);
		last_traj_no = stod(vec[0]);
	}

	
	

    return 0;
}