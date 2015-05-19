/////////////////////////////////////////////////////////////////////////////
// DataStore.cpp - Support storage of Catalogue							   //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Jim Fawcett, 2012                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    ASUS Q550L, Core i7, Windows 8.1                           //
// Application: Summer Projects, 2012                                      //
// Author:      Gautam Peri, CST 4-284, Syracuse University                //
//              (315) 484-6562, nperi@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////

#include "DataStore.h"
#include <iostream>

// Saves the given file to DataStore
void DataStore::save(const std::string& filename, const std::string& path)
{
	ListOfPaths lstPaths = store[filename];
	std::pair<PathIter, bool> ret = paths.insert(path);
	lstPaths.push_back(ret.first);
	store[filename] = lstPaths;
}


#ifdef TEST_DATASTORE

int main()
{
	DataStore ds;
	ds.save("one.cpp", "C:\\Users\\Gautam\\Desktop\\test\\");
	ds.save("two.cpp", "C:\\Users\\Gautam\\Desktop\\test\\");
	ds.save("one.cpp", "C:\\Users\\Gautam\\Desktop\\test1\\");

	DataStore::iterator iter;
	for (iter = ds.begin(); iter != ds.end(); iter++)
	{
		std::string file = iter->first;
		std::cout << "\n  FileName: " << file;
		DataStore::ListOfPaths lstPaths = iter->second;
		DataStore::ListOfPaths::iterator lstIter;

		for (auto filePathList : lstPaths)
			std::cout << (*filePathList).c_str() << "\n" << "\t";

	}
	std::cout << "\n\n";
	getchar();
	return 0;
}

#endif


