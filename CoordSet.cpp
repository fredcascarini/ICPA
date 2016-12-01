#include "SingletonTrajectories.h"
#include "Trajectory.h"
#include "CoordSet.h"
#include "TrajectoryPoint.h"

#include "LinearRegression.h"

#include <iostream>

#include "boost_1_62_0\boost\any.hpp"

CoordSet::CoordSet(std::vector<std::string> DataLine, Trajectory& T, SingletonTrajectories& ST)  //constructor for data coordinate sets
{
	number_of_data_points = DataLine.size() - 2.0;
	atoms = DataLine[1];
	index = ST.find_bond_type_index(atoms);
	CreateTrajPoints(DataLine, ST);
	T.add_coord_set(*this);
}

CoordSet::CoordSet(std::vector<CoordSet> setOfCSInstances) //constructor for traj type sets
{
	double size = setOfCSInstances[0].number_of_data_points;
	double number = setOfCSInstances.size();
	std::vector<double> startPoints;
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

	double min_element = *startPoints.begin();
	int min_element_location = 0;


	int index = 0;

	for (std::vector<double>::iterator it = startPoints.begin(); it != startPoints.end(); ++it) {
		double val = *it;
		if (val < min_element) {
			min_element = val;
			min_element_location = index;
		}

		index++;
	}


	for (int i = 0; i < size; ++i) {

		if (min_element_change){
			std::cout << "1";
			min_element_index++;

			while (i == min_element) { //in case there are multiple identical min elements


				startPoints[min_element_location] = setOfCSInstances[min_element_location].location_of_traj_points[min_element_index+1]; //update the limit that has been reached with the new limit
				currentTrajP[min_element_location] = setOfCSInstances[min_element_location].list_of_traj_points[min_element_index]; //update the current trajP for the one that has been maxed

				auto min_element = std::min_element(startPoints.begin(), startPoints.end()); //find the new min element
				double min_element_location = distance(startPoints.begin(), min_element); //find the location of the new min element
			}
			min_element_change = false; //min element change has been done
		}
		if (i < min_element) {
			std::cout << "2";
			std::vector<anyType> currentTimeCoSets;
			for (int iii = 0; iii < number; ++iii) { 
				currentTimeCoSets.push_back(setOfCSInstances[iii].return_atoms());  
				currentTimeCoSets.push_back(currentTrajP[iii].return_coordinate()); 
			}
		}
		if (i = min_element - 1) {
			std::cout << "3";
			min_element_change = true;
		}


	}

}

size_t CoordSet::add_traj_point(TrajectoryPoint& TrPoint) { list_of_traj_points.push_back(TrPoint);	return list_of_traj_points.size() - 1; }

std::string CoordSet::DetermineTrajType(std::vector<std::string> atom_types, std::vector<std::string> coordinates) 
{
	return "Unimplemented";
}

TrajectoryPoint CoordSet::return_traj_point(int index) { return list_of_traj_points[index]; }

void CoordSet::CreateTrajPoints(std::vector<std::string> Data, SingletonTrajectories& ST) {
	
	std::vector<double> dData;
	
	for (unsigned long i = 2; i < Data.size(); ++i) { dData.push_back(std::stod(Data[i]));}
	double start = 2;
	while (start < number_of_data_points) {
		std::vector<double> EndSlopeIntercept = GetLinearFit(dData, 0.9, 1.0, start);

		double end = (start + EndSlopeIntercept[0] < dData.size()) ? (start + EndSlopeIntercept[0]) : dData.size();
		std::vector<double>::const_iterator start_of_vec = dData.begin();
		std::vector<double> linearData(start_of_vec + start, start_of_vec + end);
		TrajectoryPoint* trajP = new TrajectoryPoint (linearData,*this,ST,EndSlopeIntercept[1],EndSlopeIntercept[2]);
		location_of_traj_points.push_back(start);
		start = end + 1;
	}
	return;
}
