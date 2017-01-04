#include "set_of_trajectories.h"
#include "trajectory.h"
#include "coord_set.h"
#include "trajectory_point.h"

#include "LinearRegression.h"

#include <iostream>
#include <regex>

#include "boost_1_62_0/boost/any.hpp"
#include "boost_1_62_0/boost/tokenizer.hpp"

using namespace std;

coord_set::coord_set(vector<string> DataLine, trajectory* T, set_of_trajectories* ST)  //constructor for data coordinate sets
{
	number_of_data_points = DataLine.size() - 2;
	atoms = split_atoms(DataLine[1]);
	index = ST->find_bond_type_index(atoms);
	int num_atoms = atoms.size();
	type = atoms.size() == 2 ? "length" : atoms.size() == 3 ? "angle" : num_atoms == 4 ? "dihedral" : "error";

	create_traj_points(DataLine, ST);
	T->add_coord_set(this);
}

coord_set::coord_set(vector<coord_set*> setOfCSInstances) //constructor for traj type sets
{
	typedef boost::any anyType;

	auto size = setOfCSInstances[0]->number_of_data_points;
	int number = setOfCSInstances.size();
	vector<int> start_points;
	vector<trajectory_point*> current_traj_p;
	auto min_element_index = 0;
	auto min_element_change = true;

	vector<string> type_set;

	for (auto i = 0; i < number; ++i) { //iterate over setOfCSInstances
		start_points.push_back(setOfCSInstances[i]->location_of_traj_points[min_element_index + 1]); //initialise startPoints with start point of SECOND section (i.e. where to read the first section up to)
		current_traj_p.push_back(setOfCSInstances[i]->list_of_traj_points[min_element_index]);
		min_element_change = false;
	}

	vector<int> min_vals;
	min_vals = find_min_val_loc(start_points);
	auto min_element = min_vals[0];
	auto min_element_location = min_vals[1];

	for (auto i = 0; i < size; ++i) {
		if (min_element_change) {
			while (i == min_element) { //in case there are multiple identical min elements
				auto current_min = start_points[min_element_location]; //establishes current min value for comparison
				auto working_min = current_min; //initialises working min to a value that will make (working_min <= current_min) true
				auto iterator = 0;

				while (working_min <= current_min) { //finds the next minimum value
					if (iterator == setOfCSInstances[min_element_location]->location_of_traj_points.size() - 1) { //if it has reached the end of the list of minimums, sets the next minimum as the end of the data
						iterator++;
						working_min = size;
						break;
					}
					working_min = setOfCSInstances[min_element_location]->location_of_traj_points[iterator + 1]; //update the limit that has been reached with the next limit
					iterator++; //steps up iterator - this needs to be undone if the while loop ends
				}

				start_points[min_element_location] = working_min; //updates the startPoints vector with the found minimum
				current_traj_p[min_element_location] = setOfCSInstances[min_element_location]->list_of_traj_points[iterator - 1]; //update the current trajP for the one that has been maxed
				min_vals.clear(); //clears previous assignment from find_min_val_loc
				min_vals = find_min_val_loc(start_points);
				min_element = min_vals[0];	min_element_location = min_vals[1]; //saves new values
			}
			min_element_change = false; //min element change has been done
		}

		if (i < min_element) {
			vector<anyType> current_time_co_sets;
			for (auto iii = 0; iii < number; ++iii) { 
				current_time_co_sets.push_back(setOfCSInstances[iii]->return_atoms());  
				current_time_co_sets.push_back(current_traj_p[iii]->return_coordinate());
				current_time_co_sets.push_back(current_traj_p[iii]->return_intercept());
				current_time_co_sets.push_back(current_traj_p[iii]->return_slope());
				current_time_co_sets.push_back(current_traj_p[iii]->return_bound());
				auto type = determine_traj_type(current_time_co_sets);
				type_set.push_back(type);
			}
		}
		if (i == min_element - 1) {
			min_element_change = true;
		}
	}
	cout << type_set.size() << "\n";
}


vector<string> coord_set::split_atoms(string atoms) {

	boost::char_separator<char> sep("_");
	typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;

	vector<string> result;

	Tokenizer tok(atoms, sep);
	result.assign(tok.begin(), tok.end());

	return result;
}

inline vector<int> coord_set::find_min_val_loc(vector<int> arr) {
	auto min_element = *arr.begin();
	auto min_element_location = 0;
	auto index = 0;

	for (auto it = arr.begin(); it != arr.end(); ++it) {
		auto val = *it;
		if (val < min_element) {
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

size_t coord_set::add_traj_point(trajectory_point* TrPoint) { list_of_traj_points.push_back(TrPoint);	return list_of_traj_points.size() - 1; }

string coord_set::determine_traj_type(vector<boost::any> traj_details) const
{
	regex hcl_test("(CL_HM).");
	regex c1_test("CL_C1");
	regex c2_test("CL_C2");

	vector<bool> d_test;
	auto c1_testbool = false;


	for (unsigned int i = 0; i < traj_details.size(); i += 5) {
		auto test = traj_details[i];
		auto bond = boost::any_cast<vector<string>>(traj_details[i]);
		auto bond_as_string = bond[0];
		for (auto ii = 1; ii != bond.size(); ++ii) {
			bond_as_string = bond_as_string + "_" + bond[ii];
		}
		if (regex_match(bond_as_string, hcl_test) == 1) {
			d_test.push_back(boost::any_cast<bool>(traj_details[i + 4])); //test if Cl is bound to H
		}
		if (regex_match(bond_as_string, c1_test) == 1 || regex_match(bond_as_string, c2_test) == 1) {
			d_test.push_back(boost::any_cast<double>(traj_details[i + 3]) > 0.0);  //test if Cl is travelling away from each carbon
		}
		if (regex_match(bond_as_string, c1_test) == 1)
		{
			c1_testbool = boost::any_cast<bool>(traj_details[i + 4]);
		}

	}

	auto d_testbool = true;
	for (unsigned int ii = 0; ii != d_test.size(); ++ii) {
		if (d_test[ii] == false) { d_testbool = false; }
	}

	if (d_testbool && c1_testbool) { return "Err"; }
	if (d_testbool) { return "D"; }
	if (c1_testbool) { return "C1"; }

	return "";
}

trajectory_point* coord_set::return_traj_point(int index) { return list_of_traj_points[index]; }

void coord_set::create_traj_points(vector<string> Data, set_of_trajectories* ST) {
	
	vector<double> d_data;
	
	for (unsigned long i = 2; i < Data.size(); ++i) { d_data.push_back(stod(Data[i]));}
	auto start = 2;
	while (start < number_of_data_points) {
		auto end_slope_intercept = get_linear_fit(d_data, 0.9, 1.0, start);
		unsigned int e_s_iend = static_cast<int>(round(end_slope_intercept[0]));
		auto end = start + e_s_iend < d_data.size() ? start + e_s_iend : d_data.size();
		vector<double>::const_iterator start_of_vec = d_data.begin();
		vector<double> linear_data(start_of_vec + start, start_of_vec + end);
		auto traj_p = new trajectory_point (linear_data,this,ST,end_slope_intercept[1],end_slope_intercept[2]);
		location_of_traj_points.push_back(start);
		start = end + 1;
	}
}
