/////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Gets the command line arguments and  prepares catalogue    //
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

#include "..\DataStore\DataStore.h"
#include "..\FileManager\FileManager.h"
#include "..\Display\Display.h"
#include "Executive.h"
#include<iostream>
#include<string>
#include <sstream>

// Gets the user options and Patterns from the command line args
void Executive::GetOptionsAndPatternsFromCmdLine(int argc, char* argv[])
{
	for (int index = 2; index < argc; index++)
	{
		int ptr = 0;
		std::string args = argv[index];
		while (args[ptr] != '\0')
		{
			if (args[ptr] == '/')
			{
				Options_.push_back(args);

				if (args[ptr + 1] == 'f')
				{
					if (index + 1 < argc)
					{
						searchText = argv[index + 1];
						index++;
					}

				}

				break;
			}
			ptr++;
			int length = args.size();
			if (ptr == length)
				Patterns_.push_back(args);
		}
	}

	if (Patterns_.size() == 0)
	{
		Patterns_.push_back("*.*");
	}

	if (argc < 2)
	{
		path_ = ".";
	}
	else
	{
		path_ = argv[1];
	}

}

// prepares the file catalogue and searches for given text
void Executive::PrepareCatalogue()
{
	bool IsRecursive = false;
	DataStore ds;
	FileManager Fm(path_, ds);
	Fm.addAllPatterns(Patterns_);
	Arguments::iterator argIter = std::find(Options_.begin(), Options_.end(), "/s");
	if (argIter != Options_.end())
	{
		IsRecursive = true;
	}
	Fm.search(IsRecursive);

	Display dp;
	dp.DisplayCatalogue(ds);

	argIter = std::find(Options_.begin(), Options_.end(), "/d");
	if (argIter != Options_.end())
	{
		dp.DisplayDuplicates(ds);
	}

	argIter = std::find(Options_.begin(), Options_.end(), "/f");
	if (argIter != Options_.end())
	{
		Fm.TextSearch(ds, searchText);
		dp.DisplaySearchFiles(ds, searchText);
	}

	DoSearch(Fm, ds, dp);
}

// searches for given text and patterns on the catalogue
void Executive::DoSearch(FileManager& Fm, DataStore& ds, Display& dp)
{
	std::string searchString, filePatterns;
	while (1)
	{
		std::cout << "Enter Search String: ";
		std::getline(std::cin, searchString);

		std::cout << "Enter File Patterns: ";
		std::getline(std::cin, filePatterns);

		Fm.addAllPatterns(make_sentence(filePatterns));

		Fm.PatternTextSearch(ds, searchString);
		dp.DisplaySearchFiles(ds, searchString);
	}
}

// prepares the user input patterns to vector
std::vector<std::string> Executive::make_sentence(std::string s)
{
	std::string test = "";
	std::vector<std::string> v;
	int ptr = 0;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] != ' ')
			test += s[i];

		else if (s[i] == ' ')
		{
			ptr++;
			v.push_back(test);
			test = "";
		}
	}
	if (ptr == 0)
		v.push_back(test);

	return v;
}


#ifdef TEST_EXECUTIVE

int main(int argc, char* argv[])
{
	try
	{
		Executive obj;
		obj.GetOptionsAndPatternsFromCmdLine(argc, argv);
		obj.PrepareCatalogue();
	}
	catch (int e)
	{
		std::cout << "An exception occurred. Exception Nr. " << e << '\n';
	}
}

#endif