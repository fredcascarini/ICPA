#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>

#include <ctime>

#include "set_of_trajectories.h"
#include "trajectory.h"
#include "coord_set.h"

using namespace std;
using namespace boost;

set_of_trajectories* s = set_of_trajectories::instance();

vector<string> StringSplit(string s, string delimiter)
{
	size_t pos;

	string token;

	vector<string> SplitString;

	while ((pos = s.find(delimiter)) != string::npos)
	{
		token = s.substr(0, pos);
		SplitString.push_back(token);
		s = s.substr(pos + delimiter.length());
	}

	return SplitString;
}


int main()
{
	clock_t start;
	start = clock();
	double duration;
	double duration2;

	duration = clock() / static_cast<double>(CLOCKS_PER_SEC);

	string data("file_icp.dat");

	ifstream input(data.c_str());

	//file fails to open: print error message and quit
	if (!input.is_open())  { cout << "Error in opening input file"; return 1; }

	char_separator<char> sep(" ");
	typedef tokenizer<char_separator<char>> Tokenizer;

	vector<vector<string>> vecTraj;
	vector<string> vec;
	string line;
	double last_traj_no = 0.0;
	bool newTraj;

	newTraj = true;

	auto tracker = 0;

	while (getline(input, line))
	{
		auto print_on = 100;

		if (tracker % print_on == 0)
		{
			duration2 = (clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
			cout << tracker / print_on << "\t" << (duration2 - duration) * 1000 << "ms\n";
			duration = duration2;
		}

		++tracker;

		Tokenizer tok(line, sep);
		vec.assign(tok.begin(), tok.end());
		if (newTraj)
		{
			vecTraj.push_back(vec);
			last_traj_no = stod(vec[0]);
			newTraj = false;
			continue;
		}
		if (stod(vec[0]) == last_traj_no)
		{
			vecTraj.push_back(vec);
			continue;
		}
		auto traj = trajectory(s, vecTraj);
		traj.analyse();
		vecTraj.clear();
		vecTraj.push_back(vec);
		last_traj_no = stod(vec[0]);
	}

	cout << (clock() - start) / static_cast<double>(CLOCKS_PER_SEC) << "s";

	return 0;
}
