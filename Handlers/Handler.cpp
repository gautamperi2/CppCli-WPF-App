/////////////////////////////////////////////////////////////////////////////
// Handler.cpp - Handler file to receive and process the socket stream		//
// Ver 1.1                                                                 //
// Jim Fawcett, CSE#687 - Object Oriented Design, Spring 2015              //
// CST 4-187, Syracuse University, 315 443-3948, jfawcett@twcny.rr.com     //
//-------------------------------------------------------------------------//
// Gautam peri (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Demo for Project #4, Communication Channel                 //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013					//
//Author:      Gautam Nagesh Peri, CST 4 - 484, 3154846562				  //
//              nperi@syr.edu											  ////
/////////////////////////////////////////////////////////////////////////////
#include "Handler.h"
#include "Communication.h"
#include <iostream>
#include <fstream>
#include "FileSystem.h"
#include "../FileManager/FileManager.h"
#include <vector>

using namespace FileSystem;

//----< Client Handler thread starts running this function >-----------------

void ClientHandler::operator()(Socket& socket_)
{
	std::thread dequeueThread(&ClientHandler::DequeueAndProcess, this, std::ref(socket_));
	dequeueThread.detach();
	while (true)
	{
		// interpret test command		
		std::string command = socket_.recvString('>');
		if (command.size() == 0)
			break;
		this->objQueue.EnQ(command);

	}
	this->objQueue.EnQ("quit");
	::Sleep(1000);
}

// Dequeues from the Blocking queue and process received message
void ClientHandler::DequeueAndProcess(Socket& socket_)
{
	try
	{
		std::string msg;
		while (this->objQueue.size() == 0)
		{
			::Sleep(100);
		}
		CommMessage obj;
		while (true)
		{
			msg = this->objQueue.DeQ();

			if (msg != "quit")
			{
				DeFrameMessage(msg, obj);
			}
			else
				break;
		}

		FinishProcessing(obj, socket_);
	}
	catch (std::exception& ex)
	{
		Verbose v(true);
		Verbose::show("  Exception caught:", true);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

void ShutDown(Socket& socket_)
{
	Verbose::show("ClientHandler socket connection closing");
	socket_.shutDown();
	socket_.close();
	Verbose::show("ClientHandler thread terminating");
}

// processes completely received messages to perform actions
void ClientHandler::FinishProcessing(CommMessage& obj, Socket& socket_)
{
	if (obj.GetCommand() == "TEST_FILE_HANDLING"){
		this->FileTransferHandler(obj);
	}
	else if (obj.GetCommand() == "TEST_STRING_HANDLING")	{
		std::cout << "\nRECEIVED STRING FROM " << obj.GetSrcPort() << "\n" << obj.GetMsgBody() << "\n\n";
	}
	else if (obj.GetCommand() == "TEST_FILE_RECEIVED")	{
		std::cout << "\n\nAcknowlegement:" << "\n" << "FILE_RECEIVED: " << obj.GetFileName() << " FROM " << obj.GetSrcPort() << " \n\n";
	}
	else if (obj.GetCommand() == "GET_FILES_SERVER"){
		//return files from server
		this->SendFileListing(obj, "POST_FILES_SERVER");
	}
	else if (obj.GetCommand() == "POST_FILES_SERVER")	{
		std::cout << "\n POST_FILES_SERVER FROM : " << obj.GetSrcPort() << "\n";
		this->FileListHandler(obj);
	}

	FinishProcessing1(obj, socket_);
	ShutDown(socket_);
}

// an extension to finishProcessing1
void ClientHandler::FinishProcessing1(CommMessage& obj, Socket& socket_)
{
	if (obj.GetCommand() == "GET_FILE_DOWNLOAD")	{
		this->FileDownloadHandler(obj, "TEST_FILE_HANDLING");
	}
	else if (obj.GetCommand() == "SEARCH_FOR_TEXT")	{
		std::cout << "\n SEARCH_FOR_TEXT:" << "\n" << obj.GetMsgBody() << " FROM " << obj.GetSrcPort() << " \n\n";
		this->SendTextSearchList(obj, "SEND_TEXTSEARCH_LIST");
	}
	else if (obj.GetCommand() == "SEARCH_TEXT_FILES")	{
		std::cout << "\n SEARCH_TEXT_FILES:" << "\n" << obj.GetMsgBody() << " FROM " << obj.GetSrcPort() << " \n\n";
		this->SendFileSearchList(obj, "SEND_TEXTFILES_LIST");
	}
	else if (obj.GetCommand() == "SEND_TEXTSEARCH_LIST"){
		std::cout << "\n SEND_TEXTSEARCH_LIST FROM : " << obj.GetSrcPort() << "\n";
		this->FileListHandler(obj);
	}
	else if (obj.GetCommand() == "SEND_TEXTFILES_LIST"){
		std::cout << "\n SEND_TEXTFILES_LIST FROM : " << obj.GetSrcPort() << "\n";
		this->FileListHandler(obj);
	}
}

// Send Acknowlegement after task completion
void ClientHandler::SendAck(CommMessage& obj, std::string str)
{
	Utilities objUt;
	Sender objSend;
	SocketSystem ss;
	objSend.Connect(obj.GetSrcIP(), std::stoi(obj.GetSrcPort()));
	std::string header = objUt.FrameMessage(str, std::stoi(obj.GetSrcPort()), obj.GetSrcIP(), std::stoi(obj.GetDestPort()), obj.GetDestIP(), obj.GetFileName());
	objSend.SendAck(header);
}

// trims the input to search text
std::string trim(std::string const& str)
{
	if (str.empty())
		return str;

	std::size_t first = str.find_first_not_of(' ');
	std::size_t last = str.find_last_not_of('\0');
	return str.substr(first, last - first + 1);
}

// gets file data from FileManager
std::string GetFileData(DataStore& store)
{
	std::string fileInfo;
	DataStore::iterator iter;
	for (iter = store.begin(); iter != store.end(); iter++)
	{
		std::string file = iter->first;
		fileInfo = fileInfo + file + ";";
	}

	return fileInfo;
}

// gets FileInfo from manager
std::string GetFileInfo()
{
	DataStore ds;
	std::string path_ = ".";
	FileManager Fm(path_, ds);
	Fm.search();
	std::string fileInfo = GetFileData(ds);
	return fileInfo;
}

// gets the file list from name search
std::string GetSearchFilesList(DataStore& store)
{
	std::string fileInfo;
	std::set<std::string>::iterator iter;
	for (iter = store.setBegin(); iter != store.setEnd(); iter++)
	{
		std::string file = *iter;
		fileInfo = fileInfo + file + ";";
	}
	return fileInfo;
}

// gets the file list from text search
std::string SearchFileText(std::string searchText)
{
	DataStore ds;
	std::string path_ = ".";
	FileManager Fm(path_, ds);
	Fm.search(true);
	Fm.TextSearch(ds, trim(searchText));
	std::cout << "Doing Text Search\n";
	std::string fileInfo = GetSearchFilesList(ds);
	std::cout << "File Output:\n" << fileInfo;
	return fileInfo;
}

// gets the file list to search
std::string GetFileDetailsToSearch(std::string searchText)
{
	DataStore ds;
	std::string path_ = ".";
	FileManager Fm(path_, ds);
	Fm.addPattern(trim(searchText));
	Fm.search(true);
	std::string fileInfo = GetFileData(ds);
	return fileInfo;
}

// Sends the search results to client
void ClientHandler::SendFileSearchList(CommMessage& obj, std::string str)
{
	Utilities objUt;
	Sender objSend;
	std::string header = objUt.FrameMessage(str, std::stoi(obj.GetSrcPort()), obj.GetSrcIP(), std::stoi(obj.GetDestPort()), obj.GetDestIP(), obj.GetFileName());
	std::string fileList = GetFileDetailsToSearch(obj.GetMsgBody());
	fileList = header + fileList;

	objSend.EnQString(fileList);
	objSend.EnQString("quit");
	std::thread deqThread(&Sender::GetDataAndSend, std::ref(objSend), obj.GetSrcIP(), std::stoi(obj.GetSrcPort()));
	deqThread.join();

}

// searches for text in files and returns them.
void ClientHandler::SendTextSearchList(CommMessage& obj, std::string str)
{
	Utilities objUt;
	Sender objSend;
	std::string header = objUt.FrameMessage(str, std::stoi(obj.GetSrcPort()), obj.GetSrcIP(), std::stoi(obj.GetDestPort()), obj.GetDestIP(), obj.GetFileName());
	std::string fileList = SearchFileText(obj.GetMsgBody());
	fileList = header + fileList;

	objSend.EnQString(fileList);
	objSend.EnQString("quit");
	std::thread deqThread(&Sender::GetDataAndSend, std::ref(objSend), obj.GetSrcIP(), std::stoi(obj.GetSrcPort()));
	deqThread.join();
}

// Send Acknowlegement after task completion
void ClientHandler::SendFileListing(CommMessage& obj, std::string str)
{
	std::cout << "\n GET_FILES_SERVER" << obj.GetSrcPort() << "\n" << obj.GetMsgBody() << "\n\n";
	Utilities objUt;
	Sender objSend;
	std::string header = objUt.FrameMessage(str, std::stoi(obj.GetSrcPort()), obj.GetSrcIP(), std::stoi(obj.GetDestPort()), obj.GetDestIP(), obj.GetFileName());
	std::string fileList = GetFileInfo();
	fileList = header + fileList;

	objSend.EnQString(fileList);
	objSend.EnQString("quit");
	std::thread deqThread(&Sender::GetDataAndSend, std::ref(objSend), obj.GetSrcIP(), std::stoi(obj.GetSrcPort()));
	deqThread.join();
}

void ClientHandler::FileDownloadHandler(CommMessage& obj, std::string str)
{
	std::cout << "\n GET_FILE_DOWNLOAD" << obj.GetSrcPort() << "\n";
	Utilities objUt;
	Sender objSend;
	std::string header = objUt.FrameMessage(str, std::stoi(obj.GetSrcPort()), obj.GetSrcIP(), std::stoi(obj.GetDestPort()), obj.GetDestIP(), obj.GetFileName());
	objSend.SendFile(obj.GetFileName(), header);
	objSend.EnQString("quit");

	std::thread deqThread(&Sender::GetDataAndSend, std::ref(objSend), obj.GetSrcIP(), std::stoi(obj.GetSrcPort()));
	deqThread.join();
}

// Deframe the recived message and store it in an object
void ClientHandler::DeFrameMessage(std::string msg, CommMessage& obj)
{
	std::string command = msg.substr(msg.find("COMMAND:") + 8, msg.find("DEST_PORT:") - (msg.find("COMMAND:") + 8) - 1);
	std::string destPort = msg.substr(msg.find("DEST_PORT:") + 10, msg.find("DEST_IP:") - (msg.find("DEST_PORT:") + 10) - 1);
	std::string destIP = msg.substr(msg.find("DEST_IP:") + 8, msg.find("SRC_PORT:") - (msg.find("DEST_IP:") + 8) - 1);
	std::string srcPort = msg.substr(msg.find("SRC_PORT:") + 9, msg.find("SRC_IP:") - (msg.find("SRC_PORT:") + 9) - 1);
	std::string srcIP = msg.substr(msg.find("SRC_IP:") + 7, msg.find("FILENAME:") - (msg.find("SRC_IP:") + 7) - 1);
	std::string fileName = msg.substr(msg.find("FILENAME:") + 9, msg.find("MSG_BODY:") - (msg.find("FILENAME:") + 9) - 2);
	std::string MessageBody = msg.substr(msg.find("MSG_BODY:") + 9, msg.length() - (msg.find("MSG_BODY:") + 9));


	obj.SetCommand(command);
	obj.SetDestPort(destPort);
	obj.SetDestIP(destIP);
	obj.SetSrcPort(srcPort);
	obj.SetSrcIP(srcIP);
	obj.SetFileName(fileName);
	obj.SetMsgBody(obj.GetMsgBody() + MessageBody);
}

//creates directory on Source port and saves the recived file to that path
void ClientHandler::FileTransferHandler(CommMessage& obj)
{
	if (!Directory::exists(obj.GetSrcPort()))
	{
		Directory::create(obj.GetSrcPort());
	}

	std::string path = ".\\" + obj.GetSrcPort() + "\\" + obj.GetFileName();
	std::ofstream myfile;
	myfile.open(path, std::ios::out | std::ios::binary);
	myfile << obj.GetMsgBody();
	myfile.close();

	std::cout << "FILE_RECEIVED: " << obj.GetFileName() << " FROM " << obj.GetSrcPort() << " \n\n";
	std::cout << "\n\nAcknowlegement Sent For:" << obj.GetFileName() << "to" << obj.GetSrcPort() << "\n";
}

// Splits the string into tokens
void ClientHandler::split(const std::string& string, const std::string& delimiter, std::vector<std::string>& tokens)
{
	std::string::size_type i = 0;
	std::string::size_type j = string.find(delimiter);
	while (j != std::string::npos) {
		tokens.push_back(string.substr(i, j - i));
		i = ++j;
		j = string.find(delimiter, j);
		if (j == std::string::npos)
			tokens.push_back(string.substr(i, string.length()));
	}
}

// Sends the files present on Servert to client
void ClientHandler::FileListHandler(CommMessage& obj)
{
	this->split(obj.GetMsgBody(), ";", fileList);
	for (size_t index = 0; index < fileList.size(); index++)
	{
		std::cout << "\n" << fileList[index];
		blockQ->enQ(fileList[index]);
	}
	fileList.clear();
}

// Displays list of files on Client
void ClientHandler::DisplayFileList()
{

	for (size_t index = 0; index < fileList.size(); index++)
	{
		std::cout << "\n" << fileList[index];
	}

}

// Starts receiver to connect sockets and listen
void Receiver::Connect(size_t port, ClientHandler& cp)
{
	SocketSystem ss;
	SocketListener sl(port, Socket::IP6);
	sl.start(cp);

	ApplicationHelpers::Verbose::show("press key to exit: ", ApplicationHelpers::always);
	std::cout.flush();
	std::cin.get();
}

// Starts receiver to connect server sockets and listen
void Receiver::ConnectServer(size_t port)
{

	SocketSystem ss;
	SocketListener sl(port, Socket::IP6);
	ClientHandler cp;
	sl.start(cp);

	ApplicationHelpers::Verbose::show("press key to exit: ", ApplicationHelpers::always);
	std::cout.flush();
	std::cin.get();
}


#ifdef TEST_HANDLER
int main()
{

}
#endif
