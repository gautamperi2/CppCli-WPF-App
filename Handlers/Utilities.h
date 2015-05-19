#ifndef QUEUEHANDLER_H
#define QUEUEHANDLER_H
///////////////////////////////////////////////////////////////////
// Handler.h - Handler to receive and process requests            //
// Ver 1.3                                                       //
// Application: Pr#3, Spring 2015					             //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013		  //
// Author:      Gautam Nagesh Peri, CST 4-484, 3154846562        //
//              nperi@syr.edu		                            //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package has two major classes CommMessage to deframe and save message
and Client Handler to receive the socket stream and process them.
*
*
* public interface:
*----------------------
*std::string header = obj.FrameMessage("TEST_STRING_HANDLING", DestiPort, DestiIP, SrcPort, SrcIP, "");
*

* Required Files:
* ---------------
*   - Cpp11-BlockingQueue.h, AppHelpers.h,
*     Sockets.h
*
* Build Process:
* --------------
*   devenv Sockets.sln /debug rebuild
*
* Maintenance History:
* --------------------
*
* Ver 1.0 : 21 Apr 15
* - first release
*/

#include <stdlib.h>
#include "Cpp11-BlockingQueue.h"

class Utilities
{
public:
	std::string FrameMessage(std::string Command, size_t DestiPort, std::string DestiIP, size_t SrcPort, std::string SrcIP, std::string FileName);
};

class QueueHandler
{
public:
	void EnQ(std::string strMessage){ bQ.enQ(strMessage); }
	size_t size(){ return bQ.size(); }
	std::string DeQ(){ return bQ.deQ(); }

	void SendThread(size_t DestiPort, std::string DestiIP);
private:
	BlockingQueue<std::string> bQ;
};

#endif