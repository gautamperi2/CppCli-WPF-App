
// FileMgr.cpp - Collects file specifications from directory tree

#include "FileManager.h"
#include <iostream>

void FileManager::search()
{
	find(path_);
}


void FileManager::find(const std::string& path)
{
	//std::cout << "\n  Path is: " << path;
	for (auto patt : patterns_)
	{
		std::string fullPath = FileSystem::Path::getFullFileSpec(path);
		std::vector<std::string> files = FileSystem::Directory::getFiles(fullPath, patt);
		for (auto f : files)
		{
			//std::cout << "\n  file is: " << f;
			store_.save(f, fullPath);
		}
		std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fullPath);

		// remove "." and ".." from dirs

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

		for (auto d : dirs)
		{
			std::string dirPath = FileSystem::Path::getFullFileSpec(d);
			find(dirPath);
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
	/*fm.addPattern("*.pdf");
	fm.addPattern("*.txt");
	fm.addPattern("*.obj");*/
	fm.addPattern("*.*");
	fm.search();

	DataStore::iterator iter;
	for (iter = ds.begin(); iter != ds.end(); iter++)
	{
		std::string file = iter->first;
		std::cout << "\n  FileName: \n" << file <<"\n";
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
