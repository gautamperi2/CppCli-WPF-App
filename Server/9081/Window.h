pace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#inc COMMAND:TEST_FILE_HANDLING,DEST_PORT:9080,DEST_IP:localhost,SRC_PORT:9081,SRC_IP:localhost,FILENAME:Window.h

MSG_BODY:lude <iostream);
    TabItem^ hConnectTab = gcnew TabItem();
    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();

    // Controls for SendMessage View

    Grid^ hSendMessageGrid = gcnew Grid();
    Button^ hSendButton = gcnew Button();
    Button^ hClearButton = gcnew Button();
    TextBlock^ hTextBlock1 = gcnew TextBlock();
    ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
    StackPanel^ hStackPanel1 = COMMAND:TEST_FILE_HANDLING,DEST_PORT:9080,DEST_IP:localhost,SRC_PORT:9081,SRC_IP:localhost,FILENAME:Window.h

MSG_BODY: gcnew StackPanel();

    String^ msgText 
      = "Command:ShowMessage\n"   // command
      + "Sendr:localhost@8080\n"  // send address
      + "Recvr:localhost@8090\n"  // receive address
      + "Content-length:44\n"     // body length attribute
      + "\n"                      // end header
      + "Hello World\nCSE687 - Object Oriented Design";  // message body

    // Controls for FileListView View

    Grid^ hFileListGrid = gcnew Grid();
    Forms::FolderBrowserDialog^ hFolderBrowserDi COMMAND:TEST_FILE_HANDLING,DEST_PORT:9080,DEST_IP:localhost,SRC_PORT:9081,SRC_IP:localhost,FILENAME:Window.h

MSG_BODY:alog = gcnew Forms::FolderBrowserDialog();
    ListBox^ hListBox = gcnew ListBox();
    Button^ hFolderBrowseButton = gcnew Button();
    Grid^ hGrid2 = gcnew Grid();

    // receive thread

    Thread^ recvThread;

  public:
    WPFCppCliDemo();
    ~WPFCppCliDemo();

    void setUpStatusBar();
    void setUpTabControl();
    void setUpSendMessageView();
    void setUpFileListView();
    void setUpConnectionView();

    void sendMessage(Object^ obj, RoutedEventArgs^ args);
    void addT COMMAND:TEST_FILE_HANDLING,DEST_PORT:9080,DEST_IP:localhost,SRC_PORT:9081,SRC_IP:localhost,FILENAME:Window.h

MSG_BODY:ext(String^ msg);
    void getMessage();
    void clear(Object^ sender, RoutedEventArgs^ args);
    void browseForFolder(Object^ sender, RoutedEventArgs^ args);
    void OnLoaded(Object^ sender, RoutedEventArgs^ args);
    void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
  private:
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
    void setTextBlockProperties();
    void setButtonsProperties();
  };
}


#endif
 void addT 