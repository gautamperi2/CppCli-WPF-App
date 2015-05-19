#ifndef EXECUTIVE_H
#define EXECUTIVE_H

/////////////////////////////////////////////////////////////////////////////
// Executive.h - Gets the command line arguments and  prepares catalogue    //
//				and searches for a given text							  //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Gautam Peri, 2012                                           //
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
* This module gets the command line arguments and processes them 
* and executes prepare catalogue.
* It supports the Search text from the catalogue and sends the result to
* display package.
*
* Public Interface:
* =================
* Executive obj;
* obj.GetOptionsAndPatternsFromCmdLine(argc, argv);
* obj.PrepareCatalogue();

* std:: string searchText =- "Seacrh"
* std:: string args =- "*.* *.cpp"
* obj.DoSearch(searchText);
* obj.make_sentence(args);
*
* Required Files:
* ===============
* Executive.h, Executive.cpp
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

class Executive
{
	using Arguments = std::vector < std::string > ;
	using Path = std::string;


public:
	void  GetOptionsAndPatternsFromCmdLine(int argc, char* argv[]);
	void  PrepareCatalogue();
	void  TextSearch(DataStore& ds);
	void  DoSearch(FileManager& Fm, DataStore& ds, Display& dp);
	Arguments make_sentence(std::string s);

private:
	Arguments Options_;
	Arguments Patterns_;
	Path path_;
	std::string searchText;

};


#endif