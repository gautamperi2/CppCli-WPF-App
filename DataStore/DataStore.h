#ifndef DATASTORE_H
#define DATASTORE_H
/////////////////////////////////////////////////////////////////////////////
// DataStore.h - Support storage of Catalogue							   //
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
/*
* Module Operations:
* ==================
* This module provides storage and retrival of DataStore elements.
*
* Public Interface:
* =================
* DataStore ds;
* ds.save("one.cpp", "C:\\Users\\Gautam\\Desktop\\test\\");
* ds.save("two.cpp", "C:\\Users\\Gautam\\Desktop\\test\\");
* ds.save("one.cpp", "C:\\Users\\Gautam\\Desktop\\test1\\");
*
* DataStore::iterator iter;
* for (iter = ds.begin(); iter != ds.end(); iter++)
* {
* std::string file = iter->first;
* std::cout << "\n  FileName: " << file;
* DataStore::ListOfPaths lstPaths = iter->second;
* DataStore::ListOfPaths::iterator lstIter;
*
* for (auto filePathList : lstPaths)
* std::cout << (*filePathList).c_str() << "\n" << "\t";
*
* }
*
* Required Files:
* ===============
* DataStore.h, DataStore.cpp
*
* Build Command:
* ==============
* devenv FileCatalogue.sln /rebuild Debug
*
* Maintenance History:
* ====================

* ver 1.0 : 12 Feb 15
* - first release
*/

#include <vector>
#include <string>
#include <set>
#include <list>
#include <map>

class DataStore
{

public:

	using Path = std::string;
	using Paths = std::set < Path > ;
	using PathIter = Paths::iterator;
	using ListOfPaths = std::list < PathIter > ;
	using File = std::string;
	using Store = std::map < File, ListOfPaths > ;
	using StoreList = std::set < Path > ;
	using iterator = Store::iterator;
	using StoreListIter = StoreList::iterator;

	void save(const std::string& filespec);
	void save(const std::string& filename, const std::string& path);
	iterator begin() { return store.begin(); }
	iterator end() { return store.end(); }

	StoreListIter setBegin() { return list_.begin(); }
	StoreListIter setEnd() { return list_.end(); }
	StoreList GetSearchList(){ return list_; }

	void AddToSearchList(const std::string& filename)
	{
		list_.insert(filename);
	}
	void ClearSearchList()
	{
		list_.clear();
	}

	int PathSize()
	{
		return paths.size();
	}

	int StoreSize()
	{
		return store.size();
	}

private:
	Store store;
	Paths paths;
	StoreList list_;
	StoreList files_;

};


#endif