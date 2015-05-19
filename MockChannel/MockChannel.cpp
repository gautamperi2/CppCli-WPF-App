/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - build as DLL to show how C++\CLI client can use native code channel
//				Sender to Send all inputs to queues and sockets		//
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

#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "../Handlers/Communication.h"
#include "../Handlers/Handler.h"
#include <string>
#include <thread>
#include <iostream>
#include <vector>

using BQueue = BlockingQueue < Message > ;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const Message& msg);
	BQueue& queue();
	void GetFiles(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort);
	void Connect(const std::string& ip, size_t port)
	{
		this->objSend.Connect(ip, port);
	}
	void SendFile(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string filePath);
	void DownLoadFile(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string filePath);
	void SearchText(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string searchText);
	void SearchFiles(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string searchText);

private:
	BQueue sendQ_;
	Sender objSend;
};

//Sends the message to Sender queue
void Sendr::postMessage(const Message& msg)
{
	sendQ_.enQ(msg);
}

//Get file request to the server
void Sendr::GetFiles(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort)
{
	Utilities objUt;
	std::string msg = objUt.FrameMessage("GET_FILES_SERVER", std::stoi(ServerPort), ServerIP, std::stoi(ClientPort), ClientIP, "");
	this->objSend.EnQString(msg + "SampleText");
	this->objSend.EnQString("quit");
	this->objSend.SendThread(std::stoi(ServerPort), ServerIP);

	std::cout << "Files Requested\n";
}

//Upload file request to server
void Sendr::SendFile(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string filePath)
{
	Utilities objUt;
	std::string header = objUt.FrameMessage("TEST_FILE_HANDLING", std::stoi(ServerPort), ServerIP, std::stoi(ClientPort), ClientIP, filePath);
	objSend.SendFile(filePath, header);
	objSend.EnQString("quit");
	this->objSend.SendThread(std::stoi(ServerPort), ServerIP);
}

//Download file request to server
void Sendr::DownLoadFile(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string filePath)
{
	Utilities objUt;
	std::string header = objUt.FrameMessage("GET_FILE_DOWNLOAD", std::stoi(ServerPort), ServerIP, std::stoi(ClientPort), ClientIP, filePath);
	this->objSend.EnQString(header);
	this->objSend.EnQString("quit");
	this->objSend.SendThread(std::stoi(ServerPort), ServerIP);
}

//Search files with given search text
void Sendr::SearchText(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string searchText)
{
	Utilities objUt;
	std::string header = objUt.FrameMessage("SEARCH_FOR_TEXT", std::stoi(ServerPort), ServerIP, std::stoi(ClientPort), ClientIP, "");
	this->objSend.EnQString(header + searchText);
	this->objSend.EnQString("quit");
	this->objSend.SendThread(std::stoi(ServerPort), ServerIP);
}

//Search files with expressions
void Sendr::SearchFiles(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string searchText)
{
	Utilities objUt;
	std::string header = objUt.FrameMessage("SEARCH_TEXT_FILES", std::stoi(ServerPort), ServerIP, std::stoi(ClientPort), ClientIP, "");
	this->objSend.EnQString(header + searchText);
	this->objSend.EnQString("quit");
	this->objSend.SendThread(std::stoi(ServerPort), ServerIP);
}

//Sender blocking queue message from the blocking queue
BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	Message getMessage();
	BQueue& queue();
	Recvr();
	void ConnectReceiver(size_t port)
	{
		this->objRec.Connect(port, cp);
	}

	void CheckFileList()
	{
		if (this->cp.GetFileList().size() != 0)
		{
			std::vector <std::string> files = cp.GetFileList();
			for (size_t index = 0; index < files.size(); index++)
			{
				this->blockQ.enQ(files[index]);
			}
			cp.Clear();
		}
	}

	void GetFileList(){}

private:
	//BQueue recvQ_;
	BlockingQueue <std::string> blockQ;
	Receiver objRec;
	ClientHandler cp;
};


//Gets message from the blocking queue
Message Recvr::getMessage()
{
	return blockQ.deQ();
}

//receiver blocking queue
BQueue& Recvr::queue()
{
	return blockQ;
}

//receiver constructor
Recvr::Recvr() : cp(&blockQ)
{
}

/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
	MockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();
private:
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	std::cout << "\n  MockChannel starting up";
	std::cout << "\n  We are here";
	thread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
		{
			std::cout << "\n  failed to start Mock Channel\n\n";
			return;
		}

		BQueue& sendQ = pSendr->queue();
		BQueue& recvQ = pRecvr->queue();
		while (!stop_)
		{
			std::cout << "\n  channel deQing message";
			Message msg = sendQ.deQ();  // will block here so send quit message when stopping
			std::cout << "\n  channel enQing message";
			recvQ.enQ(msg);
		}
		std::cout << "\n  Server stopping\n\n";
	});
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
	return new MockChannel(pISendr, pIRecvr);
}


#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
	ObjectFactory objFact;
	ISendr* pSendr = objFact.createSendr();
	IRecvr* pRecvr = objFact.createRecvr();
	IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
	pMockChannel->start();
	pSendr->postMessage("Hello World");
	pSendr->postMessage("CSE687 - Object Oriented Design");
	Message msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pSendr->postMessage("stopping");
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pMockChannel->stop();
	pSendr->postMessage("quit");
	std::cin.get();
}
#endif
