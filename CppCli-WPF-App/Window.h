#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//          - Project - 4(Remote Code Management)                       //
// ver 2.1                                                               //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015             //
// Modified By:  Ojas Juneja - Syracuse University                       //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It 
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
* devenv CppCli-WPF-App.sln / rebuild debug
*  - this builds C++\CLI client application and channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 2.1: 26 Apr 2015
*  - Done major changes to UI according to project requirements
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with mock channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;
using namespace System::Collections::Generic;
using namespace System::IO;


#include "../Channel/Channel.h"
#include <iostream>
#include <vector>
#include<sstream>
#include<chrono>
namespace CppCliWindows
{
	

  ref class WPFCppCliDemo : Window
  {
    // MockChannel references

	IChannel* pChann_;
	IClient* pClient_;
    // Controls for Window
    DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
    TabItem^ hSendMessageTab = gcnew TabItem();
    TabItem^ hFileListTab = gcnew TabItem();
    TabItem^ hUploadFileTab = gcnew TabItem();
	TabItem^ hFileSearchTab = gcnew TabItem();
	TabItem^ hHelpTab = gcnew TabItem();
    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();

	//Controls for File Upload
	Grid^ hUploadFileGrid = gcnew Grid();
	Button^ hBrowseButton = gcnew Button();
	TextBlock^ hTextBlockFileUp = gcnew TextBlock();
	CheckBox^ checkBoxServer1 = gcnew CheckBox();
	CheckBox^ checkBoxServer2 = gcnew CheckBox();
	ScrollViewer^ hScrollViewerFileUp = gcnew ScrollViewer();
	Label^ hLabel = gcnew Label();
	Label^ hServer1 = gcnew Label();
	Label^ hServer2 = gcnew Label();
	Label^ hCategory1 = gcnew Label();
	Label^ hCategory2 = gcnew Label();
	Label^ blankLabel1 = gcnew Label();
	Label^ blankLabel2 = gcnew Label();
	Label^ blankLabel3 = gcnew Label();
	String^ selectedFile;
	Forms::OpenFileDialog^ hFileBrowserDialog = gcnew Forms::OpenFileDialog();
	ListBox^ hUpListBox = gcnew ListBox();
	Button^ hUpDownloadButton = gcnew Button();
	Button^ hUpClearButton = gcnew Button();
	ComboBox^ hComboBoxServer1 = gcnew ComboBox();
	ComboBox^ hComboBoxServer2 = gcnew ComboBox();
	StackPanel^ hStackPanelServerOptions = gcnew StackPanel();
	StackPanel^ hStackPanelLabel = gcnew StackPanel();
	StackPanel^ hStackPanelButton = gcnew StackPanel();
	StackPanel^ hDCStackPanel = gcnew StackPanel();
	StackPanel^ hStackPanelUpload = gcnew StackPanel();
	StackPanel^ hBlankStackPanel = gcnew StackPanel();
	StackPanel^ hMainStack = gcnew StackPanel();
	
	//Controls for File Search
	Grid^ hFileSearchGrid = gcnew Grid();
	Button^ hFileSearchButton = gcnew Button();
	Button^ hTextSearchButton = gcnew Button();
	Button^ hFsDownloadButton = gcnew Button();
	Button^ hFsClearButton = gcnew Button();
	Button^ hSendFileButton = gcnew Button();
	TextBox^ hTextBoxFileUpPatt = gcnew TextBox();
	TextBox^ hTextBoxFileUpSearch = gcnew TextBox();
	StackPanel^ mainStack = gcnew StackPanel();
	StackPanel^ hFsBlankStackPanel = gcnew StackPanel();
	StackPanel^ hFsStackPanelSearch = gcnew StackPanel();
	StackPanel^ hFsStackPanelSearchOptions = gcnew StackPanel();
	StackPanel^ hFsStackPanelSearchButton = gcnew StackPanel();
	StackPanel^ hFsStackPanelOptions = gcnew StackPanel();
	StackPanel^ hFsDCStackPanel = gcnew StackPanel();
	CheckBox^ fsCheckBoxServer1 = gcnew CheckBox();
	CheckBox^ fsCheckBoxServer2 = gcnew CheckBox();
	ScrollViewer^ hScrollViewerFileSearch = gcnew ScrollViewer();
	Label^ fsLabelServer1 = gcnew Label();
	Label^ fsLabelServer2 = gcnew Label();
	Label^ fsBlankLabel11 = gcnew Label();
	Label^ fsLabelServer1Category = gcnew Label();
	Label^ fsLabelServer2Category = gcnew Label();
	Label^ fsLabelpattern = gcnew Label();
	Label^ fsLabelTextSearch = gcnew Label();
	Label^ fsBlankLabel12 = gcnew Label();
	Label^ fsBlankLabel13 = gcnew Label();
	ListBox^ hFsListBox = gcnew ListBox();
	ComboBox^ comboBoxServer1 = gcnew ComboBox();
	ComboBox^ comboBoxServer2 = gcnew ComboBox();

	//Controls for Help Tab
	Grid^ hHelpGrid = gcnew Grid();
	Label^ hHelpLabel = gcnew Label();
    // receive thread

    Thread^ recvThread;

  public:
    WPFCppCliDemo(array<System::String^>^ args);
    ~WPFCppCliDemo();

  private:
	  int server1Port, server2Port;
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
	void setUpStatusBar();
	void setUpTabControl();
	void setUpFileUploadView();
	void setUpFileSearchView();
	void setUpHelpView();
	void addText(array<String^>^ msg);
	void getMessage();
	void clear(Object^ sender, RoutedEventArgs^ args);
	void clearFile(Object^ sender, RoutedEventArgs^ args);
	void browseForFiles(Object^ sender, RoutedEventArgs^ args);
	void OnLoaded(Object^ sender, RoutedEventArgs^ args);
	void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	void getParameters(Object^ sender, RoutedEventArgs^ args);
	void getParametersTextsearch(Object^ sender, RoutedEventArgs^ args);
	void callDownloadFile(Object^ sender, RoutedEventArgs^ args);
	//properties for file upload
	void setFileUploadPanel();
	void setStackPanelsUpload(Border^ panelSearch, Border^ borderBrowseButton, Border^ panelListBox, Border^ borderClearButton);
	void setLabelsUpload();
	void setCheckBoxAndComboBoxUpload();
	void setBlankLabelsUpload();
	//properties for file search
	void setFileSearchPanel();
	void setFileSearchLabels();
	void setFileSearchTextCheckComboBoxes();
	void setOrientationFileSearch();
	void setStackPanelsFileSearch(Border^ panelSearch, Border^ borderFileSearchButton, Border^ borderTextSearchButton);
	void setMainStackPanelsFileSearch(Border^ panelSearch);
	//properties for Help
	void setUpHelpPanel();
  };
  std::chrono::steady_clock::time_point start;
}


#endif
