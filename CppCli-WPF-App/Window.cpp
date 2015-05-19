/////////////////////////////////////////////////////////////////////////////
// Window.cpp -				GUI for CSE 687 Project #4 	//
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

#include "Window.h"
using namespace CppCliWindows;

//constructor for the GUI application
WPFCppCliDemo::WPFCppCliDemo()
{
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start();
	delete pObjFact;

	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();

	// set event handlers

	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UploadFile);
	hDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::FileDownload);
	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	hConnectButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::ConnectServer);
	hTxtButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SearchText);
	hFileButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SearchFiles);

	// set Window properties

	this->Title = "Remote Code Management";
	this->Width = 800;
	this->Height = 600;

	// attach dock panel to Window

	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);

	setUpTabControl();
	setUpStatusBar();
	setUpFileManageView();
	setUpConnectionView();
}

//destructor for the GUI application
WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}

//sets up the status bar
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//creates the tab control
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hSendMessageTab->Header = "Manage Files";
	hTabControl->Items->Add(hSendMessageTab);
}

//sets the text block properties for ports 
void WPFCppCliDemo::setTextBlockProperties()
{
	Console::Write("\n  setting up FileList view");
	hSendMessageGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hSendMessageGrid;

	RowDefinition^ hRow0Def = gcnew RowDefinition();
	hRow0Def->Height = GridLength(50);
	hSendMessageGrid->RowDefinitions->Add(hRow0Def);

	hSPortTxt->Padding = Thickness(15);
	hSPortTxt->Text = "Server Port:";
	hSPortTxt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSPortTxt->FontSize = 13;
	hSPortTxt->Margin = Thickness(20, 0, 0, 0);
	hSendMessageGrid->SetRow(hSPortTxt, 0);
	hSendMessageGrid->Children->Add(hSPortTxt);

	hConnPortTxt->Padding = Thickness(15);
	hConnPortTxt->Text = "9080";
	hConnPortTxt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hConnPortTxt->FontSize = 13;
	hConnPortTxt->Width = 100;

	hListenPortTxt->Padding = Thickness(15);
	hListenPortTxt->Text = "9081";
	hListenPortTxt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hListenPortTxt->FontSize = 13;
	hListenPortTxt->Width = 100;

	setTextBlockProperties0();
	setTextBlockProperties1();
	setTextBlockProperties2();
	setTextBlockProperties3();
}

//sets the text block properties for connect 
void WPFCppCliDemo::setTextBlockProperties0()
{
	Border^ hBorderPort = gcnew Border();
	hBorderPort->BorderThickness = Thickness(1);
	hBorderPort->BorderBrush = Brushes::Black;
	hBorderPort->Child = hConnPortTxt;
	hBorderPort->Height = 50;
	hBorderPort->Width = 50;
	hBorderPort->Margin = Thickness(250, 0, 0, 0);
	hSendMessageGrid->SetRow(hBorderPort, 0);
	hSendMessageGrid->Children->Add(hBorderPort);

	Border^ hBorderPort1 = gcnew Border();
	hBorderPort1->BorderThickness = Thickness(1);
	hBorderPort1->BorderBrush = Brushes::Black;
	hBorderPort1->Child = hListenPortTxt;
	hBorderPort1->Height = 50;
	hBorderPort1->Width = 50;
	hBorderPort1->Margin = Thickness(400, 0, 0, 0);
	hSendMessageGrid->SetRow(hBorderPort1, 0);
	hSendMessageGrid->Children->Add(hBorderPort1);

	hConnectButton->Content = "Connect";
	hConnectButton->Height = 30;
	hConnectButton->Width = 120;
	hConnectButton->BorderThickness = Thickness(2);
	hConnectButton->BorderBrush = Brushes::Black;
	hSendMessageGrid->SetRow(hConnectButton, 0);
	hSendMessageGrid->Children->Add(hConnectButton);
}

//sets the text block properties for Browse 
void WPFCppCliDemo::setTextBlockProperties1()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);

	hBrowseTxt->Padding = Thickness(15);
	hBrowseTxt->Text = "Browse local directory:";
	hBrowseTxt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hBrowseTxt->FontSize = 13;
	hBrowseTxt->Margin = Thickness(20, 0, 0, 0);
	hSendMessageGrid->SetRow(hBrowseTxt, 1);
	hSendMessageGrid->Children->Add(hBrowseTxt);

	hFolderBrowseButton->Content = "Select Directory";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 120;
	hFolderBrowseButton->Margin = Thickness(20, 0, 200, 0);
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hSendMessageGrid->SetRow(hFolderBrowseButton, 1);
	hSendMessageGrid->Children->Add(hFolderBrowseButton);


	hServerTxt->Padding = Thickness(15);
	hServerTxt->Text = "Server directory:";
	hBrowseTxt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hServerTxt->FontSize = 13;
	hServerTxt->Margin = Thickness(400, 0, 0, 0);
	hSendMessageGrid->SetRow(hServerTxt, 1);
	hSendMessageGrid->Children->Add(hServerTxt);
}

//sets the Listboxesk properties
void WPFCppCliDemo::setTextBlockProperties2()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);

	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox;
	hBorder1->Margin = Thickness(20, 0, 400, 0);
	hSendMessageGrid->SetRow(hBorder1, 2);
	hSendMessageGrid->Children->Add(hBorder1);

	Border^ hBorder2 = gcnew Border();
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hListBox2;
	hBorder2->Margin = Thickness(400, 0, 20, 0);
	hSendMessageGrid->SetRow(hBorder2, 2);
	hSendMessageGrid->Children->Add(hBorder2);

	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//sets the file and text search properties
void WPFCppCliDemo::setTextBlockProperties3()
{
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow4Def);
	hFileButton->Content = "Search Files";

	hSearchTxt->Padding = Thickness(15);
	hSearchTxt->Text = "*.*";
	hSearchTxt->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTxt->FontSize = 13;

	Border^ hBorderPort = gcnew Border();
	hBorderPort->BorderThickness = Thickness(1);
	hBorderPort->BorderBrush = Brushes::Black;
	hBorderPort->Child = hSearchTxt;
	hBorderPort->Height = 50;
	hBorderPort->Margin = Thickness(20, 0, 30, 0);
	hStackPanel2->Children->Add(hBorderPort);


	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hTxtButton->Content = "Search Text";
	hBorder2->Child = hFileButton;


	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hTxtButton;
	hStackPanel2->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hBorder3);
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSendMessageGrid->SetRow(hStackPanel2, 4);
	hSendMessageGrid->Children->Add(hStackPanel2);
}

//sets the upload and download properties
void WPFCppCliDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hUploadButton->Content = "Upload File";

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hDownloadButton->Content = "Download file";
	hBorder2->Child = hUploadButton;


	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownloadButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder3);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSendMessageGrid->SetRow(hStackPanel1, 3);
	hSendMessageGrid->Children->Add(hStackPanel1);
}

//sets the file manage view
void WPFCppCliDemo::setUpFileManageView()
{
	Console::Write("\n  setting up sendMessage view");
	hSendMessageGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hSendMessageGrid;

	setTextBlockProperties();
	setButtonsProperties();
}

//Converts the string to std::string 
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

//Sends message to server mock channel queue
void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
	pSendr_->postMessage(toStdString(msgText));
	Console::Write("\n  sent message");
	hStatus->Text = "Sent message";
}

//Event handler for file upload
void WPFCppCliDemo::UploadFile(Object^ obj, RoutedEventArgs^ args)
{
	try
	{
		String^ servPort = hConnPortTxt->Text;
		String^ clientPort = hListenPortTxt->Text;
		if (hListBox->SelectedIndex != -1)
		{
			String^ file = hListBox->SelectedValue->ToString();
			std::string checkFile = toStdString(file);

			pSendr_->SendFile("localhost", toStdString(servPort), "localhost", toStdString(clientPort), toStdString(file));
			Console::Write("\n  Uploading File...");
			hStatus->Text = "Uploading File...";
		}
		else
		{
			Console::Write("\n  Empty file...");
			hStatus->Text = "Select a file from Directory";
		}
	}
	catch (std::exception)
	{
		Console::Write("Exception:");		
	}

}

//Event handler for file download
void WPFCppCliDemo::FileDownload(Object^ obj, RoutedEventArgs^ args)
{
	String^ servPort = hConnPortTxt->Text;
	String^ clientPort = hListenPortTxt->Text;
	pSendr_->DownLoadFile("localhost", toStdString(servPort), "localhost", toStdString(clientPort), "11245.txt");
	Console::Write("\n  Downloading file..");
	hStatus->Text = "Downloading file..";
}

//Event handler for search text in files
void WPFCppCliDemo::SearchText(Object^ obj, RoutedEventArgs^ args)
{
	String^ servPort = hConnPortTxt->Text;
	String^ clientPort = hListenPortTxt->Text;
	String^ searchText = hSearchTxt->Text;
	pSendr_->SearchText("localhost", toStdString(servPort), "localhost", toStdString(clientPort), toStdString(searchText));
	Console::Write("\n  Searching files for text..");
	hStatus->Text = "Searching files for text..";
}

//Event handler for search files
void WPFCppCliDemo::SearchFiles(Object^ obj, RoutedEventArgs^ args)
{
	String^ servPort = hConnPortTxt->Text;
	String^ clientPort = hListenPortTxt->Text;
	String^ searchText = hSearchTxt->Text;
	pSendr_->SearchFiles("localhost", toStdString(servPort), "localhost", toStdString(clientPort), toStdString(searchText));
	Console::Write("\n  Searching file names for text..");
	hStatus->Text = "Searching file names for text..";
}

//Converts to system string from std::string
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

//Event handler to add text to textblock
void WPFCppCliDemo::addText(String^ msg)
{
	hTextBlock1->Text += msg + "\n\n";
}

//Event handler to display files in listbox
void WPFCppCliDemo::DisplayFiles(String^ msg)
{
	hStatus->Text = "Connecting to Dequeue";
	hListBox2->Items->Add(msg);
}

//Event handler to Get message from receiver queue
void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function
	pRecvr_->ConnectReceiver(9081);
	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		Console::Write("\n  cleared message text");
		//pRecvr_->CheckFileList();

		std::string msg = pRecvr_->getMessage();

		std::cout << "\n  looping through..!!";
		String^ sMsg = toSystemString(msg);
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;

		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::DisplayFiles);
		Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	}
}

//Event handler to clear text from textblock
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock1->Text = "";
}

//Event handler to connectServer button click
void WPFCppCliDemo::ConnectServer(Object^ sender, RoutedEventArgs^ args)
{
	String^ servPort = hConnPortTxt->Text;
	String^ clientPort = hListenPortTxt->Text;
	Console::Write("\n  Connecting to Server");
	hStatus->Text = "Connecting to Server...";
	pSendr_->GetFiles("localhost", toStdString(servPort), "localhost", toStdString(clientPort));
}

//sets up the file list view on GUI
void WPFCppCliDemo::setUpFileListView()
{
	Console::Write("\n  setting up FileList view");
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFolderBrowseButton->Content = "Select Directory";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 120;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 1);
	hFileListGrid->Children->Add(hFolderBrowseButton);

	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//browses the files from the browse folder
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
	}
}

//sets up the connection view
void WPFCppCliDemo::setUpConnectionView()
{
	Console::Write("\n  setting up Connection view");
}

//Event Handler for GUI on load 
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}

//Event Handler for GUI on unload 
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	try 
	{
		Console::WriteLine(L"\n Starting WPFCppCliDemo");

		Application^ app = gcnew Application();
		app->Run(gcnew WPFCppCliDemo());
		Console::WriteLine(L"\n\n");
	}
	catch (std::exception)
	{
		Console::WriteLine("Exception");
	}
}