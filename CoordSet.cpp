#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

#include "LinearRegression.h"

#include <iostream>
#include <regex>

#include "boost_1_62_0/boost/any.hpp"
#include "boost_1_62_0/boost/tokenizer.hpp"

using namespace std;

CoordSet::CoordSet(vector<string> dataLine, SingletonTrajectories* ST)  //constructor for data coordinate sets
	: data_line_(dataLine), st_(ST) {
	number_of_data_points = dataLine.size() - num_header_cols;
	name = dataLine[1];
	atoms = SplitAtoms(name);
	index = ST->find_bond_type_index(atoms);
	int num_atoms = atoms.size();

	switch (num_atoms) {
	case (2):
		{
			type = dataType::length;
			break;
		}
	case (3):
		{
			type = dataType::angle;
			break;
		}
	case (4):
		{
			type = dataType::dihedral;
			break;
		}
	default:
		{
			type = dataType::error;
			break;
		}
	}
}

void CoordSet::analyse() {
	CreateTrajPoints(data_line_, st_);
}

CoordSet::CoordSet(vector<CoordSet*> setOfCSInstances) //constructor for traj type sets
{
	typedef boost::any anyType;

	auto size = setOfCSInstances[0]->number_of_data_points;
	int number = setOfCSInstances.size();
	vector<int> startPoints;
	vector<TrajectoryPoint*> currentTrajP;
	auto min_element_index = 0;
	auto min_element_change = true;

	vector<string> type_set;

	for (auto i = 0; i < number; ++i) { //iterate over setOfCSInstances
		if (setOfCSInstances[i]->location_of_traj_points.size() != 1 ) {
			startPoints.push_back(setOfCSInstances[i]->location_of_traj_points[min_element_index + 1]); //initialise startPoints with start point of SECOND section (i.e. where to read the first section up to)
		}
		else {
			startPoints.push_back(size);
		}
		currentTrajP.push_back(setOfCSInstances[i]->list_of_traj_points[min_element_index]);
		min_element_change = false;
	}

	vector<int> min_vals;
	min_vals = find_min_val_loc(startPoints);
	auto min_element = min_vals[0];
	auto min_element_location = min_vals[1];

	for (auto i = 0; i < size; ++i) {
		if (min_element_change) {
			while (i == min_element) { //in case there are multiple identical min elements
				auto current_min = startPoints[min_element_location]; //establishes current min value for comparison
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

				startPoints[min_element_location] = working_min; //updates the startPoints vector with the found minimum
				currentTrajP[min_element_location] = setOfCSInstances[min_element_location]->list_of_traj_points[iterator - 1]; //update the current trajP for the one that has been maxed
				min_vals.clear(); //clears previous assignment from find_min_val_loc
				min_vals = find_min_val_loc(startPoints);
				min_element = min_vals[0];	min_element_location = min_vals[1]; //saves new values
			}
			min_element_change = false; //min element change has been done
		}

		if (i < min_element) {
			vector<anyType> currentTimeCoSets;
			for (auto iii = 0; iii < number; ++iii) { 
				currentTimeCoSets.push_back(setOfCSInstances[iii]->return_atoms());  
				currentTimeCoSets.push_back(currentTrajP[iii]->return_coordinate());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_intercept());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_slope());
				currentTimeCoSets.push_back(currentTrajP[iii]->return_bound());
				auto type = DetermineTrajType(currentTimeCoSets);
				type_set.push_back(type);
			}
		}
		if (i == min_element - 1) {
			min_element_change = true;
		}
	}
	cout << type_set.size() << "\n";
}


vector<string> CoordSet::SplitAtoms(string atoms) {

	boost::char_separator<char> sep("_");
	typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;

	vector<string> result;

	Tokenizer tok(atoms, sep);
	result.assign(tok.begin(), tok.end());

	return result;
}

inline vector<int> CoordSet::find_min_val_loc(vector<int> arr) {
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

size_t CoordSet::add_traj_point(TrajectoryPoint* TrPoint) { list_of_traj_points.push_back(TrPoint);	return list_of_traj_points.size() - 1; }

string CoordSet::DetermineTrajType(vector<boost::any> traj_details) const
{
	regex HCL_test("(CL_HM).");
	regex C1_test("CL_C1");
	regex C2_test("CL_C2");

	vector<bool> Dtest;
	auto C1testbool = false;


	for (unsigned int i = 0; i < traj_details.size(); i += 5) {
		auto test = traj_details[i];
		auto bond = boost::any_cast<vector<string>>(traj_details[i]);
		auto bond_as_string = bond[0];
		for (auto ii = 1; ii != bond.size(); ++ii) {
			bond_as_string = bond_as_string + "_" + bond[ii];
		}
		if (regex_match(bond_as_string, HCL_test) == 1) {
			Dtest.push_back(boost::any_cast<bool>(traj_details[i + 4])); //test if Cl is bound to H
		}
		if (regex_match(bond_as_string, C1_test) == 1 || regex_match(bond_as_string, C2_test) == 1) {
			Dtest.push_back(boost::any_cast<double>(traj_details[i + 3]) > 0.0);  //test if Cl is travelling away from each carbon
		}
		if (regex_match(bond_as_string, C1_test) == 1)
		{
			C1testbool = boost::any_cast<bool>(traj_details[i + 4]);
		}

	}

	auto DtestBool = true;
	for (unsigned int ii = 0; ii != Dtest.size(); ++ii) {
		if (Dtest[ii] == false) { DtestBool = false; }
	}

	if (DtestBool && C1testbool) { return "Err"; }
	if (DtestBool) { return "D"; }
	if (C1testbool) { return "C1"; }

	return "";
}

TrajectoryPoint* CoordSet::return_traj_point(int index) { return list_of_traj_points[index]; }

void CoordSet::CreateTrajPoints(vector<string> Data, SingletonTrajectories* ST) {
	
	vector<double> dData;
	
	for (unsigned long i = num_header_cols; i < Data.size(); ++i) {
		dData.push_back(stod(Data[i]));
	}
	auto start = num_header_cols;
	while (start < number_of_data_points) {

		auto EndSlopeIntercept = GetLinearFit(dData, 0.9, 1.0, start);
		unsigned int ESIend = static_cast<int>(round(EndSlopeIntercept[0]));
		auto end = (start + ESIend < dData.size()) ? (start + ESIend) : dData.size();
		vector<double>::const_iterator start_of_vec = dData.begin();
		vector<double> linearData(start_of_vec + start, start_of_vec + end);
		auto trajP = new TrajectoryPoint (linearData,this,ST,EndSlopeIntercept[1],EndSlopeIntercept[2]);
		add_traj_point(trajP);
		location_of_traj_points.push_back(start);
		start = end + 1;
	}
	return;
}
