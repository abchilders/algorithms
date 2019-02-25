#pragma once
#ifndef PATH_H
#define PATH_H

template<typename T>
class Path
{
private:
	// contains a list of things representing a path
	vector<T> _path{}; 
public:
	// outputs the entire path
	void outputFullPath(ostream &out_stream);

	// outputs the path, without the first and last elements
	void outputPartialPath(ostream &out_stream); 
};

#endif //!PATH_H
