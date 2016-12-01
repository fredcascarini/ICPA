#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

#include "LinearRegression.h"

#include <iostream>

#include "boost_1_62_0\boost\any.hpp"

CoordSet::CoordSet(std::vector<std::string> DataLine, Trajectory& T, SingletonTrajectories& ST)  //constructor for data coordinate sets
{
	number_of_data_points = DataLine.size() - 2;
	atoms = DataLine[1];
	index = ST.find_bond_type_index(atoms);
	CreateTrajPoints(DataLine, ST);
	T.add_coord_set(*this);
}

CoordSet::CoordSet(std::vector<CoordSet> setOfCSInstances) //constructor for traj type sets
{
	int size = setOfCSInstances[0].number_of_data_points;
	int number = setOfCSInstances.size();
	std::vector<int> startPoints;
	std::vector<TrajectoryPoint> currentTrajP;
	typedef boost::any anyType;

	int min_element_index = 0;
	bool min_element_change;

	for (int i = 0; i < number; ++i) { //iterate over setOfCSInstances
		startPoints.push_back(setOfCSInstances[i].location_of_traj_points[min_element_index + 1]); //initialise startPoints with start point of SECOND section (i.e. where to read the first section up to)
		currentTrajP.push_back(setOfCSInstances[i].list_of_traj_points[min_element_index]);
		min_element_change = false;
	}

	/* auto min_element = std::min_element(startPoints.begin(), startPoints.end());
	double min_element_location = distance(startPoints.begin(), min_element); */

	std::vector<int> min_vals;
	min_vals = find_min_val_loc(startPoints);

	int min_element = min_vals[0];
	int min_element_location = min_vals[1];



	for (int i = 0; i < size; ++i) {
		if (min_element_change) {
			while (i == min_element) { //in case there are multiple identical min elements
				int current_min = startPoints[min_element_location]; //establishes current min value for comparison
				int working_min = current_min; //initialises working min to a value that will make (working_min <= current_min) true
				int iterator = 0;

				while (working_min <= current_min) { //finds the next minimum value
					if (iterator == setOfCSInstances[min_element_location].location_of_traj_points.size() - 1) { //if it has reached the end of the list of minimums, sets the next minimum as the end of the data
						iterator++;
						working_min = size;
						break;
					}
					working_min = setOfCSInstances[min_element_location].location_of_traj_points[iterator + 1]; //update the limit that has been reached with the next limit
					iterator++; //steps up iterator - this needs to be undone if the while loop ends
				}

				startPoints[min_element_location] = working_min; //updates the startPoints vector with the found minimum
				
				currentTrajP[min_element_location] = setOfCSInstances[min_element_location].list_of_traj_points[iterator - 1]; //update the current trajP for the one that has been maxed
				min_vals.clear(); //clears previous assignment from find_min_val_loc
				min_vals = find_min_val_loc(startPoints);
				min_element = min_vals[0];	min_element_location = min_vals[1]; //saves new values
			}
			min_element_change = false; //min element change has been done
		}

		if (i < min_element) {
			std::vector<anyType> currentTimeCoSets;
			for (int iii = 0; iii < number; ++iii) { 
				currentTimeCoSets.push_back(setOfCSInstances[iii].return_atoms());  
				currentTimeCoSets.push_back(currentTrajP[iii].return_coordinate());
				currentTimeCoSets.push_back(currentTrajP[iii].return_intercept());
				currentTimeCoSets.push_back(currentTrajP[iii].return_slope());
				currentTimeCoSets.push_back(currentTrajP[iii].return_bound());
				std::string type = DetermineTrajType(currentTimeCoSets);
			}
		}
		if (i = min_element - 1) {
			min_element_change = true;
		}


	}

}


inline std::vector<int> CoordSet::find_min_val_loc(std::vector<int> arr) {

	int min_element = *arr.begin();
	int min_element_location = 0;
	int index = 0;

	for (std::vector<int>::iterator it = arr.begin(); it != arr.end(); ++it) {
		int val = *it;
		if (val < min_element) {
			min_element = val;
			min_element_location = index;
		}

		index++;
	}

	std::vector<int> results;

	results.push_back(min_element);
	results.push_back(min_element_location);

	return results;
}

size_t CoordSet::add_traj_point(TrajectoryPoint& TrPoint) { list_of_traj_points.push_back(TrPoint);	return list_of_traj_points.size() - 1; }

std::string CoordSet::DetermineTrajType(std::vector<boost::any> traj_details) 
{
	return "Unimplemented";
}

TrajectoryPoint CoordSet::return_traj_point(int index) { return list_of_traj_points[index]; }

void CoordSet::CreateTrajPoints(std::vector<std::string> Data, SingletonTrajectories& ST) {
	
	std::vector<double> dData;
	
	for (unsigned long i = 2; i < Data.size(); ++i) { dData.push_back(std::stod(Data[i]));}
	int start = 2;
	while (start < number_of_data_points) {
		std::vector<double> EndSlopeIntercept = GetLinearFit(dData, 0.9, 1.0, start);
		unsigned int ESIend = static_cast<int>(std::round(EndSlopeIntercept[0]));
		unsigned int end = (start + ESIend < dData.size()) ? (start + ESIend) : dData.size();
		std::vector<double>::const_iterator start_of_vec = dData.begin();
		std::vector<double> linearData(start_of_vec + start, start_of_vec + end);
		TrajectoryPoint* trajP = new TrajectoryPoint (linearData,*this,ST,EndSlopeIntercept[1],EndSlopeIntercept[2]);
		location_of_traj_points.push_back(start);
		start = end + 1;
	}
	return;
}
