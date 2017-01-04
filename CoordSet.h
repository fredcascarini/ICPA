#ifndef COORDSET_H
#define COORDSET_H

#include <vector>
#include <boost/any.hpp>

class SingletonTrajectories;
class Trajectory;
class TrajectoryPoint;

enum class dataType {length, angle, dihedral, error};

class CoordSet {

public:

	CoordSet(std::vector<std::string> DataLine, SingletonTrajectories* ST);						//data set constructor
	void analyse();
	explicit CoordSet(std::vector<CoordSet*> setOfCSInstances);															//type set constructor

	std::vector<std::string>	return_atoms() const
	//return atoms relevant to this CoordSet as string
	{ return atoms; }

	int							return_index() const
	//return bond type index from ST as int
	{ return index; }

	dataType					return_type() const
	//return coordinate type as string
	{ return type; }

	static std::vector<std::string>	SplitAtoms(std::string atoms);													//Split list of atoms into vector array of atom names

	std::string return_name() const { return name; }

	size_t						add_traj_point(TrajectoryPoint* TrPoint);										//adds TrajectoryPoint instance to list_of_traj_points. Returns index of new pointer as size_t
	TrajectoryPoint*			return_traj_point(int index);													//returns TrajectoryPoint instance at index in list_of_traj_points as pointer
	void						CreateTrajPoints(std::vector<std::string> Data, SingletonTrajectories* ST);		//Separates data into a string of TrajectoryPoints
	std::string					DetermineTrajType(std::vector<boost::any> traj_details) const;						//Determines trajectory type, returns type as string
	static inline std::vector<int>		find_min_val_loc(std::vector<int> arr);											//finds minimum value in an array of ints, returns vector - vector[0] is min value, vector[1] is location


private:
	const int num_header_cols = 2;
	std::vector<std::string>		atoms;																		//string of atoms that co-ordinate refers to
	int								index;																		//bond type index from SingletonTrajectories
	dataType						type;																		//coordinate type: length, angle, dihedral etc.
	std::vector<TrajectoryPoint*>	list_of_traj_points;														//vector of pointers to TrajectoryPoint instances
	std::vector<int>				location_of_traj_points;													//vector of start indices of each traj point
	std::vector<int>				set_of_traj_types;															//vector of trajectory types (e.g. Roaming, C1 complex) as numbers
	int								number_of_data_points;														//number of individual data points in coord set
	std::vector<std::basic_string<char>> data_line_;
	SingletonTrajectories* st_;
	std::string name;
	//~CoordSet() {};
};

#endif

