/*
Assignment: PA #2 - Dijkstra's algorithm
Description: This program will use CSV information on campus walking times
	to prompt the user for two campus location codes and calculate the
	estimated shortest travel time using Dijkstra's algorithm, as well as
	outputting what buildings one will pass on the way.
Author: Alex Childers
HSU ID: 013145324
Completion time: 12.75 hours
In completing this program, I received help from the following people:
	N/A
*/

// Written by Alex Childers. 

#pragma once
#ifndef PATH_H
#define PATH_H

template<typename T>
class Path
{
private:
	// the weight of the path
	int _weight = 0; 

	// a list of things representing a path
	vector<T> _path{}; 

public: 
	Path()
	{
		// intentionally empty
	}

	Path(int the_weight, vector<string> the_path)
	{
		_weight = the_weight; 
		_path = the_path; 
	}

	void setWeight(const int new_weight)
	{
		_weight = new_weight; 
	}

	int getWeight()
	{
		return _weight; 
	}

	void setPath(const vector<T> new_path)
	{
		_path = new_path; 
	}

	vector<T> getPath()
	{
		return _path; 
	}

	// to be used in a better implementation of CampusGraph::computeShortestPath
	void addToPath(const T step)
	{
		_path.push_back(step); 
	}
};

#endif //!PATH_H
