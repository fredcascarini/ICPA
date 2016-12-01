#ifndef COORDSET_H
#define COORDSET_H

#include <string>
#include <vector>

class SingletonTrajectories;
class Trajectory;

class TrajectoryPoint;

class CoordSet {

public:

	CoordSet(std::vector<std::string> DataLine, Trajectory& T, SingletonTrajectories& ST);							//data set constructor
	CoordSet(std::vector<CoordSet> setOfCSInstances);																//type set constructor

	std::string		return_atoms()																					//return atoms relevant to this CoordSet as string
	{ return atoms; }

	int				return_index()																					//return bond type index from ST as int
	{ return index; }

	std::string		return_type()																					//return coordinate type as string
	{ return type; }


	size_t			add_traj_point(TrajectoryPoint& TrPoint);														//adds TrajectoryPoint instance to list_of_traj_points. Returns index of new pointer as size_t
	TrajectoryPoint return_traj_point(int index);																	//returns TrajectoryPoint instance at index in list_of_traj_points as pointer
	void			CreateTrajPoints(std::vector<std::string> Data, SingletonTrajectories& ST);						//Separates data into a string of TrajectoryPoints
	std::string		DetermineTrajType(std::vector<std::string> atom_types, std::vector<std::string> coordinates);	//Determines trajectory type, returns type as string
	inline std::vector<int> CoordSet::find_min_val_loc(std::vector<int> arr);										//finds minimum value in an array of ints, returns vector - vector[0] is min value, vector[1] is location


private:
	std::string						atoms;																			//string of atoms that co-ordinate refers to
	int								index;																			//bond type index from SingletonTrajectories
	std::string						type;																			//coordinate type: length, angle, dihedral etc.
	std::vector<TrajectoryPoint>	list_of_traj_points;															//vector of pointers to TrajectoryPoint instances
	std::vector<double>				location_of_traj_points;														//vector of start indices of each traj point
	std::vector<double>				set_of_traj_types;																//vector of trajectory types (e.g. Roaming, C1 complex) as numbers
	double							number_of_data_points;															//number of individual data points in coord set
	//~CoordSet() {};
};

#endif

