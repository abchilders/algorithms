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
