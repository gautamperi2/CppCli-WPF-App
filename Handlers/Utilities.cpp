
/////////////////////////////////////////////////////////////////////////////
// Handler.cpp - Handler file to receive and process the socket stream		//
// Ver 1.1                                                                 //
// Jim Fawcett, CSE#687 - Object Oriented Design, Spring 2015              //
// CST 4-187, Syracuse University, 315 443-3948, jfawcett@twcny.rr.com     //
//-------------------------------------------------------------------------//
// Gautam peri (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Demo for Project #3, Communication Channel                 //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013					//
//Author:      Gautam Nagesh Peri, CST 4 - 484, 3154846562				  //
//              nperi@syr.edu											  ////
/////////////////////////////////////////////////////////////////////////////
#include "Utilities.h"
#include "Communication.h"
#include "../ApplicationHelpers/AppHelpers.h"


//void GetDataAndSend1(std::string DestIP, size_t DestPort)
//{
//	try
//	{
//		Sender objSend;
//		SocketSystem ss;
//		objSend.Connect(DestIP, DestPort);
//
//		std::string msg;
//		while (objSend.size() == 0)
//		{
//			::Sleep(100);
//		}
//		std::cout << "\nhere i am1 \n";
//		while (true)
//		{
//			msg = objSend.DeQ();
//			if (msg != "quit")
//			{
//				objSend.sendString(msg);
//			}
//			else
//				break;
//		}
//		std::cout << "\nhere i am2 \n";
//		objSend.sendString(">>");
//		objSend.shutDownSend();
//		std::cout << "\nhere i am 3\n";
//		//std::terminate();
//	}
//	catch (std::exception& ex)
//	{
//		Verbose v(true);
//		Verbose::show("  Exception caught:", true);
//		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
//	}
//}
//
//void SendThread(size_t DestiPort, std::string DestiIP)
//{
//	std::thread deqThread(GetDataAndSend1, DestiIP, DestiPort);
//	deqThread.join();
//}




#ifdef TEST_QUEUEHANDLER
int main()
{

}
#endif
