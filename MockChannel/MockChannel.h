#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
///////////////////////////////////////////////////////////////////
// MockChannel.h -Gets input from UI and sends it to Socket communication	//
// Ver 1.0										                  //
// Application: Pr#4, Spring 2015					             //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013		  //
// Author:      Gautam Nagesh Peri, CST 4-484, 3154846562        //
//              nperi@syr.edu		                            //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package has  major classes Sender to send messages to sockets
Receiver to create connection to recive and DemoHandler to display demos
*
*
* public interface:
*----------------------
*postMessage();
*GetFiles();
*SendFile();
*DownLoadFile();
SearchText();
SearchFiles();
getMessage();
ConnectReceiver(size_t port);
CheckFileList();
*/


#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using Message = std::string;

struct ISendr
{
	virtual void postMessage(const Message& msg) = 0;
	virtual void GetFiles(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort) = 0;
	virtual void SendFile(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string filePath) = 0;
	virtual void DownLoadFile(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string filePath) = 0;
	virtual void SearchText(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string searchText) = 0;
	virtual void SearchFiles(std::string ServerIP, std::string ServerPort, std::string ClientIP, std::string ClientPort, std::string searchText) = 0;
};

struct IRecvr
{
	virtual std::string getMessage() = 0;
	virtual void ConnectReceiver(size_t port) = 0;
	virtual void CheckFileList() = 0;
};

struct IMockChannel
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
};

extern "C" {
	struct ObjectFactory
	{
		DLL_DECL ISendr* createSendr();
		DLL_DECL IRecvr* createRecvr();
		DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
	};
}

#endif


