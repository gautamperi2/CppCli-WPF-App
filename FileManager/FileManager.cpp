
/////////////////////////////////////////////////////////////////////////////
// FileManager.cpp - Collects file specifications from directory tree      //
//				  and search for a given text from a given file 		   //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Jim Fawcett, 2012                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    ASUS Q550L, Core i7, Windows 8.1                           //
// Application: Summer Projects, 2012                                      //
// Author:      Gautam Peri, CST 4-284, Syracuse University                //
//              (315) 484-6562, nperi@syr.edu                              //
/////////////////////////////////////////////////////////////////////////////

#include "FileManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <regex>

//Searches all the file to get files
void FileManager::search(bool IsRecursive)
{
	findFiles(path_, IsRecursive);
}

//Finds all the files form given path recursively
void FileManager::findFiles(const std::string& path, bool IsRecursive)
{
	for (auto patt : patterns_)
	{
		std::string fullPath = FileSystem::Path::getFullFileSpec(path);
		std::vector<std::string> files = FileSystem::Directory::getFiles(fullPath, patt);
		for (auto f : files)
		{
			store_.save(f, fullPath);
		}
		std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fullPath);
		auto iter = std::find(std::begin(dirs), std::end(dirs), ".");
		if (iter != std::end(dirs))
		{
			dirs.erase(iter);
		}
		iter = std::find(std::begin(dirs), std::end(dirs), "..");
		if (iter != std::end(dirs))
		{
			dirs.erase(iter);
		}
		if (IsRecursive)
		{
			for (auto d : dirs)
			{
				std::string dirPath = fullPath + "\\" + d;
				findFiles(dirPath, IsRecursive);
			}
		}
	}
}

//searches the data store catalogue with given search text 
void FileManager::TextSearch(DataStore& store, std::string searchText)
{
	std::cout << "SearchTEXT:" << searchText << ":IS\n";
	DataStore::iterator iter;
	store.ClearSearchList();
	for (iter = store.begin(); iter != store.end(); iter++)
	{
		std::string file = iter->first;
		DataStore::ListOfPaths lstPaths = iter->second;
		for (auto filePathList : lstPaths)
		{
			//std::cout << "I am here\n";
			std::string path = (*filePathList).c_str();
			std::string filePath = path + "\\" + file;
			//std::cout << "\n FilePath:" << filePath;
			std::string line;
			std::ifstream  fileInput;
			fileInput.open(filePath);
			if (fileInput.is_open())
			{
				while (std::getline(fileInput, line))
				{
					if (line.find(searchText, 0) != std::string::npos)
					{
						store.AddToSearchList(filePath);
						break;
					}
				}
			}
		}

	}


}

//Converts a Given pattern to Regular expression
void FileManager::ConvertToRegex(std::string& patt) {
	ChangeString(patt, "*", "(.*)");
	ChangeString(patt, "?", "(.?)");
}

//Changes a given string of one form to another by given modification
void FileManager::ChangeString(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_in = 0;
	while ((start_in = str.find(from, start_in)) != std::string::npos) {
		str.replace(start_in, from.length(), to);
		start_in += to.length();
	}
}

//Searches for the given pattern files from the catalogue and search text from the DataStore
void FileManager::PatternTextSearch(DataStore& store, std::string searchText)
{
	DataStore::iterator iter;
	store.ClearSearchList();
	for (iter = store.begin(); iter != store.end(); iter++)
	{
		std::string file = iter->first;
		DataStore::ListOfPaths lstPaths = iter->second;
		for (auto filePathList : lstPaths)
		{
			std::string path = (*filePathList).c_str();
			std::string filePath = path + "\\" + file;

			for (auto patt : patterns_)
			{
				ConvertToRegex(patt);
				std::regex e(patt);
				if (std::regex_match(file, e))
				{
					std::string line;
					std::ifstream  fileInput;
					fileInput.open(filePath);
					if (fileInput.is_open())
					{
						while (std::getline(fileInput, line))
						{
							if (line.find(searchText, 0) != std::string::npos)
							{
								store.AddToSearchList(filePath);
								break;
							}
						}
					}

				}
			}

		}

	}
}


#ifdef TEST_FILEMANAGER

int main()
{
	std::cout << "\n  Testing FileManager class";
	std::cout << "\n =======================\n";

	DataStore ds;
	FileManager fm(".", ds);

	fm.addPattern("*.*");
	fm.search();

	std::string sText = "Search";
	fm.TextSearch(ds, sText);

	std::string sPatt = "*.cpp";
	fm.ConvertToRegex(sText);
	fm.ChangeString(sPatt, "*", "(.*)");
	fm.PatternTextSearch(ds, sPatt);

	DataStore::iterator iter;
	for (iter = ds.begin(); iter != ds.end(); iter++)
	{
		std::string file = iter->first;
		std::cout << "\n  FileName: \n" << file << "\n";
		DataStore::ListOfPaths lstPaths = iter->second;
		DataStore::ListOfPaths::iterator lstIter;

		for (auto filePathList : lstPaths)
			std::cout << (*filePathList).c_str() << "\n" << "\t";

	}
	std::cout << "\n\n";
	getchar();
	std::cout << "\n\n";
	return 0;
}

#endif
