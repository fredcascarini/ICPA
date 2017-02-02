#include "set_of_trajectories.h"
#include "trajectory.h"
#include "coord_set.h"
#include "trajectory_point.h"

#include "LinearRegression.h"

#include <iostream>
#include <regex>
#include <string>

#include "boost/any.hpp"
#include "boost/tokenizer.hpp"


using namespace std;

coord_set::coord_set(vector<string> dataline, set_of_trajectories* ST) //constructor for data coordinate sets
	: data_line_(dataline), st_(ST)
{
	number_of_data_points = dataline.size() - num_header_cols;
	name = dataline[1];
	atoms = SplitAtoms(name);
	index = ST->find_bond_type_index(atoms);
	int num_atoms = atoms.size();

	switch (num_atoms)
	{
	case (2):	{ type = dataType::length; break; }
	case (3):	{ type = dataType::angle; break; }
	case (4):	{ type = dataType::dihedral; break; }
	default:	{ type = dataType::error; break; }
	}
}

void coord_set::analyse() { CreateTrajPoints(data_line_, st_); }

vector<string> coord_set::SplitAtoms(string atoms)
{
	boost::char_separator<char> sep("_");
	typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;

	vector<string> result;

	Tokenizer tok(atoms, sep);
	result.assign(tok.begin(), tok.end());

	return result;
}


size_t coord_set::add_traj_point(trajectory_point* TrPoint)
{
	list_of_traj_points.push_back(TrPoint);
	return list_of_traj_points.size() - 1;
}

trajectory_point* coord_set::return_traj_point(int index) { return list_of_traj_points[index]; }

void coord_set::CreateTrajPoints(vector<string> Data, set_of_trajectories* ST)
{
	vector<double> dData; //data as double rather than as string
	for (unsigned long i = num_header_cols; i < Data.size(); ++i) { dData.push_back(stod(Data[i])); }

	vector<double> gbdData; //gaussian blurred dData
	gbdData = gaus_blur(dData, 10, 5.0);

	auto start = num_header_cols;

	//use linreg to find first (approach) section 
	auto EndSlopeIntercept = get_linear_fit(dData, 0.90, 5.0, start);
	unsigned int ESIend = static_cast<int>(round(EndSlopeIntercept[0]));
	auto end = (start + ESIend < dData.size()) ? (start + ESIend) : dData.size();

	vector<double>::const_iterator start_of_vec = dData.begin();
	vector<double> linearData(start_of_vec + start, start_of_vec + end);
	vector<double> trajectoryData(start_of_vec + end, start_of_vec + dData.size());

	auto trajP = new trajectory_point(linearData, trajectoryData, this, ST, EndSlopeIntercept[1], EndSlopeIntercept[2]);
}
