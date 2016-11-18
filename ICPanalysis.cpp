#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <boost\tokenizer.hpp>
#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "TrajectoryPoint.h"

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

	Trajectory Traj("Cl-H", s);

	TrajectoryPoint TrajPoint(2.0, Traj);


    return 0;
}