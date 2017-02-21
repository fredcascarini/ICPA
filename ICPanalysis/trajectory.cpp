#include "set_of_trajectories.h"
#include "trajectory.h"
#include "trajectory_point.h"
#include "coord_set.h"
#include <iostream>
#include <regex>
#include <functional>

using namespace std;

trajectory::trajectory(set_of_trajectories* ST, vector<vector<string>> data_lines) //constructor
	: st_(ST), data_lines_(data_lines) { number_of_coordinates = data_lines_.size(); }

trajectory::~trajectory() { for (coord_set* value_ : list_of_coord_sets) { delete value_; } }

void trajectory::analyse()
{
	//Analysis begins:
	for (auto i = 0; i < number_of_coordinates; i++)
	{
		auto C = new coord_set(data_lines_[i], st_);
		C->analyse();
		add_coord_set(C);
	}

	analyse_coord_sets(list_of_coord_sets);

	st_->add_trajectory(this); //populate ST array of pointers	
}

void trajectory::add_coord_set(coord_set* CoSet) { list_of_coord_sets.push_back(CoSet); }

coord_set* trajectory::return_coord_set(int index) { return list_of_coord_sets[index]; }

void trajectory::analyse_coord_sets(vector<coord_set*> setOfCSInstances) const
{
	typedef boost::any anyType;


	int number_of_coordinate_sets = setOfCSInstances.size();
	auto number_of_data_points = setOfCSInstances[0]->return_num_dp();

	vector<string> type_set;

	/*
	for (auto i = 0; i < number_of_data_points; ++i)
	{
		if (i < min_element)
		{
			vector<anyType> currentTimeCoSets;
			for (auto iii = 0; iii < number_of_coordinate_sets; ++iii)
			{
				currentTimeCoSets.push_back(setOfCSInstances[iii]->return_atoms());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_lin_coordinate());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_intercept());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_slope());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_bound());
				auto type = DetermineTrajType(currentTimeCoSets);
				type_set.push_back(type);
			}
		}
	}
	*/
}

string trajectory::DetermineTrajType(vector<boost::any> traj_details) const
{
	regex HCL_test("(CL_HM).");
	regex C1_test("CL_C1");
	regex C2_test("CL_C2");

	vector<bool> Dtest;
	auto C1testbool = false;


	for (unsigned int i = 0; i < traj_details.size(); i += 5)
	{
		auto test = traj_details[i];
		auto bond = boost::any_cast<vector<string>>(traj_details[i]);
		auto bond_as_string = bond[0];
		for (auto ii = 1; ii != bond.size(); ++ii) { bond_as_string = bond_as_string + "_" + bond[ii]; }
		if (regex_match(bond_as_string, HCL_test) == 1)
		{
			Dtest.push_back(boost::any_cast<bool>(traj_details[i + 4])); //test if Cl is bound to H
		}
		if (regex_match(bond_as_string, C1_test) == 1 || regex_match(bond_as_string, C2_test) == 1)
		{
			Dtest.push_back(boost::any_cast<double>(traj_details[i + 3]) > 0.0); //test if Cl is travelling away from each carbon
		}
		if (regex_match(bond_as_string, C1_test) == 1) { C1testbool = boost::any_cast<bool>(traj_details[i + 4]); }
	}

	auto DtestBool = true;
	for (unsigned int ii = 0; ii != Dtest.size(); ++ii) { if (Dtest[ii] == false) { DtestBool = false; } }

	if (DtestBool && C1testbool) { return "Err"; }
	if (DtestBool) { return "D"; }
	if (C1testbool) { return "C1"; }

	return "";
}
