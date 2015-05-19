#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////////////
// FileManager.h - Collects file specifications from directory tree        //
//				  and search for a given text from a given file 		   //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Gautam Peri, 2012                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    ASUS Q550L, Core i7, Windows 8.1                           //
// Application: Summer Projects, 2012                                      //
// Author:      Gautam Peri, CST 4-284, Syracuse University                //
//              (315) 484-6562, nperi@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* Collects file specifications from directory tree and 
* search for a given text from a given file
* It supports the Search text from the catalogue and sends the result to
* display package.
*
* Public Interface:
* =================
* FileManager fm(".", ds);
*
* fm.addPattern("*.*");
* fm.search();
*
* std::string sText = "Search";
* fm.TextSearch(ds, sText);
*
* std::string sPatt = "*.cpp";
* fm.ConvertToRegex(sText);
* fm.ChangeString(sPatt, "*", "(.*)");
* fm.PatternTextSearch(ds, sPatt);
*
* Required Files:
* ===============
* FileManager.h, FileManager.cpp
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



#include "FileSystem.h"
#include "..\DataStore\DataStore.h"
#include <vector>
#include <string>

class FileManager
{
public:
	using iterator = DataStore::iterator;
	using patterns = std::vector < std::string > ;
	using iterator = DataStore::iterator;


	FileManager(const std::string& path, DataStore& store) : path_(path), store_(store)
	{
		patterns_.push_back("*.*");
	}
	void addPattern(const std::string& patt)
	{
		if (patterns_.size() == 1 && patterns_[0] == "*.*")
			patterns_.pop_back();
		patterns_.push_back(patt);
	}

	void addAllPatterns(patterns Patt)
	{
		patterns_ = Patt;
	}

	void FileManager::TextSearch(DataStore& store, std::string searchText);
	void search(bool IsRecursive = false);
	void findFiles(const std::string&path, bool IsRecursive = false);
	iterator begin() { return store_.begin(); }
	iterator end() { return store_.end(); }

	void ListSearchFiles(DataStore& store);
	void PatternTextSearch(DataStore& store, std::string searchText);
	void ChangeString(std::string& str, const std::string& from, const std::string& to);
	void ConvertToRegex(std::string& patt);

private:
	DataStore& store_;
	std::string path_;
	patterns patterns_;
};
#endif
