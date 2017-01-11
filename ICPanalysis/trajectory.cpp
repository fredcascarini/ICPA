#include "set_of_trajectories.h"
#include "Trajectory.h"
#include "trajectory_point.h"
#include "coord_set.h"
#include <iostream>
#include <regex>

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

void trajectory::analyse_coord_sets(std::vector<coord_set*> setOfCSInstances)
{
	typedef boost::any anyType;

	auto number_of_data_points = setOfCSInstances[0]->return_num_dp();
	int number_of_coordinate_sets = setOfCSInstances.size();
	vector<int> startPoints;
	vector<trajectory_point*> currentTrajP;
	auto min_element_index = 0;
	auto min_element_change = true;

	vector<string> type_set;
	std::cout << setOfCSInstances[0]->return_tP_list().size() << "  ";

	for (auto i = 0; i < number_of_coordinate_sets; ++i)
	{ //iterate over setOfCSInstances
		auto tp_loc = setOfCSInstances[i]->return_tP_loc();
		auto tp_list = setOfCSInstances[i]->return_tP_list();
		if (tp_loc.size() != 1)
		{
			startPoints.push_back(tp_loc[min_element_index + 1]); //initialise startPoints with start point of SECOND section (i.e. where to read the first section up to)
		}
		else { startPoints.push_back(number_of_data_points); }
		currentTrajP.push_back(tp_list[min_element_index]);
		min_element_change = false;
	}

	vector<int> min_vals;
	min_vals = find_min_val_loc(startPoints);
	auto min_element = min_vals[0];
	auto min_element_location = min_vals[1];

	for (auto i = 0; i < number_of_data_points; ++i)
	{
		if (min_element_change)
		{
			while (i == min_element)
			{ //in case there are multiple identical min elements
				auto current_min = startPoints[min_element_location]; //establishes current min value for comparison
				auto working_min = current_min; //initialises working min to a value that will make (working_min <= current_min) true
				auto iterator = 0;

				while (working_min <= current_min)
				{ //finds the next minimum value
					if (iterator == setOfCSInstances[min_element_location]->return_tP_loc().size() - 1)
					{ //if it has reached the end of the list of minimums, sets the next minimum as the end of the data
						iterator++;
						working_min = number_of_data_points;
						break;
					}
					working_min = setOfCSInstances[min_element_location]->return_tP_loc()[iterator + 1]; //update the limit that has been reached with the next limit
					iterator++; //steps up iterator - this needs to be undone if the while loop ends
				}

				startPoints[min_element_location] = working_min; //updates the startPoints vector with the found minimum
				currentTrajP[min_element_location] = setOfCSInstances[min_element_location]->return_tP_list()[iterator - 1]; //update the current trajP for the one that has been maxed
				min_vals.clear(); //clears previous assignment from find_min_val_loc
				min_vals = find_min_val_loc(startPoints);
				min_element = min_vals[0];
				min_element_location = min_vals[1]; //saves new values
			}
			min_element_change = false; //min element change has been done
		}

		if (i < min_element)
		{
			vector<anyType> currentTimeCoSets;
			for (auto iii = 0; iii < number_of_coordinate_sets; ++iii)
			{
				currentTimeCoSets.push_back(setOfCSInstances[iii]->return_atoms());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_coordinate());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_intercept());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_slope());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_bound());
				auto type = DetermineTrajType(currentTimeCoSets);
				type_set.push_back(type);
			}
		}
		if (i == min_element - 1) { min_element_change = true; }
	}
}


inline vector<int> trajectory::find_min_val_loc(vector<int> arr)
{
	auto min_element = *arr.begin();
	auto min_element_location = 0;
	auto index = 0;

	for (auto it = arr.begin(); it != arr.end(); ++it)
	{
		auto val = *it;
		if (val < min_element)
		{
			min_element = val;
			min_element_location = index;
		}

		index++;
	}

	vector<int> results;

	results.push_back(min_element);
	results.push_back(min_element_location);

	return results;
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
