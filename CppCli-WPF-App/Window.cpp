///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//          - Project - 4(Remote Code Management)                        //
// ver 2.0                                                               //
// Author     : Ojas Juneja                                             //
///////////////////////////////////////////////////////////////////////////

#include "Window.h"
#include <list>
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo(array<System::String^>^ args)
{
  // set up channel

ObjectFactory* pObjFact = new ObjectFactory;
	pClient_ = pObjFact->createClient(stoi(toStdString(args[0])));
  pChann_ = pObjFact->createChannel(pClient_);
  pChann_->start();
  delete pObjFact;
  // client's receive thread
  recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
  recvThread->Start();

  server1Port = stoi(toStdString(args[1]));
  server2Port = stoi(toStdString(args[2]));
   //set event handlers

  this->Loaded += 
    gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing += 
    gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  hUpClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
  hBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFiles);
  hFileSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getParameters);
  hTextSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getParametersTextsearch);
  hFsDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::callDownloadFile);
  hFsClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearFile);
  // set Window properties

  this->Title = "Remote Code Management";
  this->Width = 1100;
  this->Height = 620;

  // attach dock panel to Window

  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);

  // setup Window controls and views

  setUpTabControl();
  setUpStatusBar();
  //setUpFileListView();
  setUpFileUploadView();
  setUpFileSearchView();
  setUpHelpView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
  delete pClient_;
}

// <----  setup Help GUI View -->
void WPFCppCliDemo::setUpHelpView()
{
	Console::Write("\n  setting up Help view");
	hUploadFileGrid->Margin = Thickness(20);
	hHelpTab->Content = hHelpGrid;
	setUpHelpPanel();

}


// <----  setup FileUpload GUI View -->
void WPFCppCliDemo::setUpFileUploadView()
{
	Console::Write("\n  setting up Connection view");
	hUploadFileGrid->Margin = Thickness(20);
	hUploadFileTab->Content = hUploadFileGrid;
	setFileUploadPanel();
}

//set up status bar
void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  hStatus->FontWeight = FontWeights::Bold;
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//set up Tab Control
void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hUploadFileTab->Header = "Upload Files";
  hFileSearchTab->Header = "Search";
  hHelpTab->Header = "Help";
  hTabControl->Items->Add(hUploadFileTab);
  hTabControl->Items->Add(hFileSearchTab);
  hTabControl->Items->Add(hHelpTab);
}

//set up HelpPanel
void WPFCppCliDemo::setUpHelpPanel()
{
	Border^ helpPanelSearch = gcnew Border();
	helpPanelSearch->Width = 1000;
	helpPanelSearch->Height = 500;
	helpPanelSearch->BorderThickness = Thickness(1);
	helpPanelSearch->BorderBrush = Brushes::Black;
	hHelpLabel->Content = "File Upload: \n" + "User has to select any or both of the server first and then select a file after clicking on the browse button the file will be uploaded \n to the server based on the check-box selection \n"
		+ "When the file is uploaded,the log will be shown in the below list box. \n\n" +
		"File Search: \n" + "Files can be searched on 2 servers based on check-box selection.\nUser can enter pattern and has to press FileSearch button while doing File searches.\n\n" +
		"Text Search: \n" + "Text present in files can be searched on 2 servers based on check-box selection.\n" + "User can enter pattern and has to enter text and press TextSearch button while doing Text searches.\n\n"
		+ "File Download: \n" + "User has to press download button after selecting a file.\n"
		+ "When the file is downloaded,its log will be shown in the list box present in UPLOAD Tab. \n\n\n"
		+ "For more Information Please read ReadMe.txt file\n";
	hHelpLabel->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hHelpLabel->FontSize = 16;
	hHelpLabel->Height = 500;
	hHelpLabel->Width = 1000;
	helpPanelSearch->Child = hHelpLabel;
	hHelpGrid->Children->Add(helpPanelSearch);
}

//set up FileUploadPanel
void WPFCppCliDemo::setFileUploadPanel()
{
	//set propeties for labels,check box and combo box
	setBlankLabelsUpload();
	setLabelsUpload();
	setCheckBoxAndComboBoxUpload();
	//set up properties for buttons
	Border^ panelSearch = gcnew Border();
	panelSearch->Width = 1000;
	panelSearch->Height = 140;
	panelSearch->BorderThickness = Thickness(1);
	panelSearch->BorderBrush = Brushes::Black;
	hBrowseButton->Content = "Browse";
	Border^ borderBrowseButton = gcnew Border();
	borderBrowseButton->Width = 100;
	borderBrowseButton->Height = 30;
	borderBrowseButton->BorderThickness = Thickness(1);
	borderBrowseButton->BorderBrush = Brushes::Black;
	borderBrowseButton->Child = hBrowseButton;
	Border^ panelListBox = gcnew Border();
	panelListBox->BorderThickness = Thickness(1);
	panelListBox->BorderBrush = Brushes::Black;
	panelListBox->Width = 1000;
	panelListBox->Height = 310;
	panelListBox->Child = hUpListBox;
	hUpClearButton->Content = "Clear";
	Border^ borderClearButton = gcnew Border();
	borderClearButton->BorderThickness = Thickness(1);
	borderClearButton->BorderBrush = Brushes::Black;
	borderClearButton->Width = 120;
	borderClearButton->Height = 30;
	borderClearButton->Child = hUpClearButton;
	//set up stack panels
	setStackPanelsUpload(panelSearch, borderBrowseButton, panelListBox, borderClearButton);
}

//set up blank labels to adjust different ojects of UI
void WPFCppCliDemo::setBlankLabelsUpload()
{
	blankLabel1->Width = 1000;
	blankLabel2->Width = 450;
	blankLabel3->Width = 430;
}

//set up properties for checkbox and combobox
void WPFCppCliDemo::setCheckBoxAndComboBoxUpload()
{
	checkBoxServer1->Width = 20;
	checkBoxServer1->Height = 50;
	checkBoxServer2->Height = 50;
	checkBoxServer2->Width = 20;

	hComboBoxServer1->Height = 30;
	hComboBoxServer1->Width = 70;
	hComboBoxServer1->Items->Add("C++");
	hComboBoxServer1->Items->Add("PDF");
	hComboBoxServer1->Items->Add("Java");
	hComboBoxServer1->SelectedIndex = 0;
	hComboBoxServer1->VerticalAlignment = System::Windows::VerticalAlignment::Top;

	hComboBoxServer2->Height = 30;
	hComboBoxServer2->Width = 70;
	hComboBoxServer2->Items->Add("C++");
	hComboBoxServer2->Items->Add("Text");
	hComboBoxServer2->Items->Add("Java");
	hComboBoxServer2->SelectedIndex = 0;
	hComboBoxServer2->VerticalAlignment = System::Windows::VerticalAlignment::Top;
}

//set up properties for labels
void WPFCppCliDemo::setLabelsUpload()
{
	hServer1->Content = "Server 1  (localhost: 8089)";
	hServer1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hServer1->FontSize = 16;
	hServer1->Height = 66;
	hServer1->Width = 200;

	hCategory1->Content = "Category : ";
	hCategory1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hCategory1->FontSize = 16;
	hCategory1->Height = 66;
	hCategory1->Width = 90;

	hCategory2->Content = "Category : ";
	hCategory2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hCategory2->FontSize = 16;
	hCategory2->Height = 66;
	hCategory2->Width = 90;

	hServer2->Content = "Server 2   (localhost: 9000)";
	hServer2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hServer2->FontSize = 16;
	hServer2->Height = 66;
	hServer2->Width = 210;

	hLabel->Content = "All Logs related to File Upload and download will appear below: ";
	hLabel->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hLabel->FontSize = 12;
	hLabel->Height = 66;
	hLabel->Width = 350;
}

//set up stack panels to arrange labels,buttons,checkboxes and combo box in UI
void WPFCppCliDemo::setStackPanelsUpload(Border^ panelSearch, Border^ borderBrowseButton, Border^ panelListBox,Border^ borderClearButton)
{
	hMainStack->Orientation = Orientation::Vertical;
	TextBlock^ hSpacer0 = gcnew TextBlock();
	hSpacer0->Width = 10;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 30;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 150;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 30;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 10;
	hBlankStackPanel->Orientation = Orientation::Horizontal;
	hStackPanelServerOptions->Orientation = Orientation::Horizontal;
	hStackPanelButton->Orientation = Orientation::Horizontal;
	hDCStackPanel->Orientation = Orientation::Horizontal;
	hStackPanelLabel->Orientation = Orientation::Horizontal;
	hStackPanelUpload->Orientation = Orientation::Vertical;
	//adding labels,checkbox and combo box to stack panels
	hBlankStackPanel->Children->Add(blankLabel1);
	hStackPanelServerOptions->Children->Add(hSpacer0);
	hStackPanelServerOptions->Children->Add(checkBoxServer1);
	hStackPanelServerOptions->Children->Add(hServer1);
	hStackPanelServerOptions->Children->Add(hSpacer1);
	hStackPanelServerOptions->Children->Add(hCategory1);
	hStackPanelServerOptions->Children->Add(hComboBoxServer1);
	hStackPanelServerOptions->Children->Add(hSpacer2);
	hStackPanelServerOptions->Children->Add(checkBoxServer2);
	hStackPanelServerOptions->Children->Add(hServer2);
	hStackPanelServerOptions->Children->Add(hSpacer3);
	hStackPanelServerOptions->Children->Add(hCategory2);
	hStackPanelServerOptions->Children->Add(hComboBoxServer2);
	hStackPanelButton->Children->Add(blankLabel2);
	hStackPanelButton->Children->Add(borderBrowseButton);
	hStackPanelLabel->Children->Add(hLabel);
	hStackPanelUpload->Children->Add(hBlankStackPanel);
	hStackPanelUpload->Children->Add(hStackPanelServerOptions);
	hStackPanelUpload->Children->Add(hStackPanelButton);
	hStackPanelUpload->Children->Add(hStackPanelLabel);
	panelSearch->Child = hStackPanelUpload;
	hDCStackPanel->Children->Add(blankLabel3);
	hDCStackPanel->Children->Add(hSpacer4);
	hDCStackPanel->Children->Add(borderClearButton);
	//adding all stack panels vertically to main stack panel
	mainStack->Children->Add(panelSearch);
	mainStack->Children->Add(panelListBox);
	mainStack->Children->Add(hDCStackPanel);
	hUploadFileGrid->Children->Add(mainStack);
}

//set up properties for FileSearchPanel
void WPFCppCliDemo::setFileSearchPanel()
{
	//set up properties for blank labels
	fsBlankLabel11->Width = 1000;
	fsBlankLabel12->Width = 400;
	fsBlankLabel13->Width = 380;
	mainStack->Orientation = Orientation::Vertical;

	Border^ panelSearch = gcnew Border();
	panelSearch->Width = 1000;
	panelSearch->Height = 180;
	panelSearch->BorderThickness = Thickness(1);
	panelSearch->BorderBrush = Brushes::Black;
	//set up properties for buttons
	hFileSearchButton->Content = "File Search";
	Border^ borderFileSearchButton = gcnew Border();
	borderFileSearchButton->Width = 100;
	borderFileSearchButton->Height = 30;
	borderFileSearchButton->BorderThickness = Thickness(1);
	borderFileSearchButton->BorderBrush = Brushes::Black;
	borderFileSearchButton->Child = hFileSearchButton;

	hTextSearchButton->Content = "Text Search";
	Border^ borderTextSearchButton = gcnew Border();
	borderTextSearchButton->Width = 100;
	borderTextSearchButton->Height = 30;
	borderTextSearchButton->BorderThickness = Thickness(1);
	borderTextSearchButton->BorderBrush = Brushes::Black;
	borderTextSearchButton->Child = hTextSearchButton;
			
	setFileSearchLabels();
	setFileSearchTextCheckComboBoxes();
	setStackPanelsFileSearch(panelSearch,borderFileSearchButton, borderTextSearchButton);
	setMainStackPanelsFileSearch(panelSearch);

}

//set up properties for labels
void WPFCppCliDemo::setFileSearchLabels()
{
	fsLabelServer1->Content = "Server 1  (localhost: 8089)";
	fsLabelServer1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabelServer1->FontSize = 16;
	fsLabelServer1->Height = 66;
	fsLabelServer1->Width = 200;

	fsLabelServer1Category->Content = "Category : ";
	fsLabelServer1Category->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabelServer1Category->FontSize = 16;
	fsLabelServer1Category->Height = 66;
	fsLabelServer1Category->Width = 90;

	fsLabelServer2Category->Content = "Category : ";
	fsLabelServer2Category->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabelServer2Category->FontSize = 16;
	fsLabelServer2Category->Height = 66;
	fsLabelServer2Category->Width = 90;

	fsLabelServer2->Content = "Server 2   (localhost: 9000)";
	fsLabelServer2->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabelServer2->FontSize = 16;
	fsLabelServer2->Height = 66;
	fsLabelServer2->Width = 210;

	fsLabelpattern->Content = "File Pattern(Space seperated) : ";
	fsLabelpattern->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabelpattern->FontSize = 16;
	fsLabelpattern->Height = 66;
	fsLabelpattern->Width = 250;

	fsLabelTextSearch->Content = "Enter Text : ";
	fsLabelTextSearch->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	fsLabelTextSearch->FontSize = 16;
	fsLabelTextSearch->Height = 66;
	fsLabelTextSearch->Width = 110;
}

//set up properties for combo boxes,text boxes and seach text boxes
void WPFCppCliDemo::setFileSearchTextCheckComboBoxes()
{
	hTextBoxFileUpSearch->Width = 200;
	hTextBoxFileUpSearch->Height = 30;
	hTextBoxFileUpPatt->Width = 200;
	hTextBoxFileUpPatt->Height = 30;

	hTextBoxFileUpPatt->FontSize = 16;
	hTextBoxFileUpPatt->BorderBrush = Brushes::Black;
	hTextBoxFileUpPatt->VerticalAlignment = System::Windows::VerticalAlignment::Top;

	hTextBoxFileUpSearch->FontSize = 16;
	hTextBoxFileUpSearch->BorderBrush = Brushes::Black;
	hTextBoxFileUpSearch->VerticalAlignment = System::Windows::VerticalAlignment::Top;

	fsCheckBoxServer1->Width = 20;
	fsCheckBoxServer1->Height = 50;
	fsCheckBoxServer2->Height = 50;
	fsCheckBoxServer2->Width = 20;

	comboBoxServer1->Height = 30;
	comboBoxServer1->Width = 70;
	comboBoxServer1->Items->Add("C++");
	comboBoxServer1->Items->Add("PDF");
	comboBoxServer1->Items->Add("Java");
	comboBoxServer1->SelectedIndex = 0;
	comboBoxServer1->VerticalAlignment = System::Windows::VerticalAlignment::Top;

	comboBoxServer2->Height = 30;
	comboBoxServer2->Width = 70;
	comboBoxServer2->Items->Add("C++");
	comboBoxServer2->Items->Add("Text");
	comboBoxServer2->Items->Add("Java");
	comboBoxServer2->SelectedIndex = 0;
	comboBoxServer2->VerticalAlignment = System::Windows::VerticalAlignment::Top;
}

//set up properties for stack panels
void WPFCppCliDemo::setStackPanelsFileSearch(Border^ panelSearch,Border^ borderFileSearchButton, Border^ borderTextSearchButton)
{
	TextBlock^ hSpacer0 = gcnew TextBlock();
	hSpacer0->Width = 10;
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 30;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 150;
	TextBlock^ hSpacer3 = gcnew TextBlock();
	hSpacer3->Width = 30;
	TextBlock^ hSpacer4 = gcnew TextBlock();
	hSpacer4->Width = 150;
	TextBlock^ hSpacer5 = gcnew TextBlock();
	hSpacer5->Width = 10;
	//set up orientation of stackpanels
	setOrientationFileSearch();
	//arranging stack panels horizontally
	hFsBlankStackPanel->Children->Add(fsBlankLabel11);
	hFsStackPanelSearchOptions->Children->Add(hSpacer0);
	hFsStackPanelSearchOptions->Children->Add(fsCheckBoxServer1);
	hFsStackPanelSearchOptions->Children->Add(fsLabelServer1);
	hFsStackPanelSearchOptions->Children->Add(hSpacer1);
	hFsStackPanelSearchOptions->Children->Add(fsLabelServer1Category);
	hFsStackPanelSearchOptions->Children->Add(comboBoxServer1);
	hFsStackPanelSearchOptions->Children->Add(hSpacer2);
	hFsStackPanelSearchOptions->Children->Add(fsCheckBoxServer2);
	hFsStackPanelSearchOptions->Children->Add(fsLabelServer2);
	hFsStackPanelSearchOptions->Children->Add(hSpacer3);
	hFsStackPanelSearchOptions->Children->Add(fsLabelServer2Category);
	hFsStackPanelSearchOptions->Children->Add(comboBoxServer2);
	hFsStackPanelOptions->Children->Add(fsLabelpattern);
	hFsStackPanelOptions->Children->Add(hTextBoxFileUpPatt);
	hFsStackPanelOptions->Children->Add(hSpacer4);
	hFsStackPanelOptions->Children->Add(fsLabelTextSearch);
	hFsStackPanelOptions->Children->Add(hTextBoxFileUpSearch);
	hFsStackPanelSearchButton->Children->Add(fsBlankLabel13);
	hFsStackPanelSearchButton->Children->Add(borderFileSearchButton);
	hFsStackPanelSearchButton->Children->Add(hSpacer5);
	hFsStackPanelSearchButton->Children->Add(borderTextSearchButton);
	hFsStackPanelSearch->Children->Add(hFsBlankStackPanel);
	hFsStackPanelSearch->Children->Add(hFsStackPanelSearchOptions);
	hFsStackPanelSearch->Children->Add(hFsStackPanelOptions);
	hFsStackPanelSearch->Children->Add(hFsStackPanelSearchButton);
	panelSearch->Child = hFsStackPanelSearch;
}

//set up orientation for stack panels
void WPFCppCliDemo::setOrientationFileSearch()
{
	hFsBlankStackPanel->Orientation = Orientation::Horizontal;
	hFsStackPanelSearchOptions->Orientation = Orientation::Horizontal;
	hFsStackPanelSearchButton->Orientation = Orientation::Horizontal;
	hFsStackPanelOptions->Orientation = Orientation::Horizontal;
	hFsDCStackPanel->Orientation = Orientation::Horizontal;
	hFsStackPanelSearch->Orientation = Orientation::Vertical;
}

//set up main stack panel by adding horizontal stack panels vertically
void WPFCppCliDemo::setMainStackPanelsFileSearch(Border^ panelSearch)
{
	TextBlock^ hSpacer6 = gcnew TextBlock();
	hSpacer6->Width = 10;
	Border^ panelListBox = gcnew Border();
	panelListBox->BorderThickness = Thickness(1);
	panelListBox->BorderBrush = Brushes::Black;
	panelListBox->Width = 1000;
	panelListBox->Height = 274;
	panelListBox->Child = hFsListBox;

	hFsDownloadButton->Content = "Download";
	Border^ borderDownloadButton = gcnew Border();
	borderDownloadButton->BorderThickness = Thickness(1);
	borderDownloadButton->BorderBrush = Brushes::Black;
	borderDownloadButton->Width = 120;
	borderDownloadButton->Height = 30;
	borderDownloadButton->Child = hFsDownloadButton;

	hFsClearButton->Content = "Clear";
	Border^ borderClearButton = gcnew Border();
	borderClearButton->BorderThickness = Thickness(1);
	borderClearButton->BorderBrush = Brushes::Black;
	borderClearButton->Width = 120;
	borderClearButton->Height = 30;
	borderClearButton->Child = hFsClearButton;

	hFsDCStackPanel->Children->Add(fsBlankLabel12);
	hFsDCStackPanel->Children->Add(borderDownloadButton);
	hFsDCStackPanel->Children->Add(hSpacer6);
	hFsDCStackPanel->Children->Add(borderClearButton);

	hMainStack->Children->Add(panelSearch);
	hMainStack->Children->Add(panelListBox);
	hMainStack->Children->Add(hFsDCStackPanel);

	hFileSearchGrid->Children->Add(hMainStack);
}

//set up grid for file search view
void WPFCppCliDemo::setUpFileSearchView()
{
	Console::Write("\n  setting up file search view");
	hFileSearchGrid->Margin = Thickness(20);
	hFileSearchTab->Content = hFileSearchGrid;
	setFileSearchPanel();
}

//converting string of managed code to native code
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

//converting string of native code to managed code
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

//showing results to UI
void WPFCppCliDemo::addText(array<String^>^ msg)
{	
	if (msg->Length != 0)
	{
		if (toStdString(msg[0]).find("uploaded") != std::string::npos)
		{
			for (int i = 0; i < msg->Length; i++)
				hUpListBox->Items->Add(msg[i]);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			typedef std::chrono::duration<int, std::milli> millisecs_t; millisecs_t duration(std::chrono::duration_cast<millisecs_t>(end - start));
			hStatus->Text = "Uploading Done in " + duration.count() + " milliseconds.";
		}
		else if (toStdString(msg[0]).find("downloaded") != std::string::npos)
		{
			for (int i = 0; i < msg->Length; i++)
				hUpListBox->Items->Add(msg[i]);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			typedef std::chrono::duration<int, std::milli> millisecs_t; millisecs_t duration(std::chrono::duration_cast<millisecs_t>(end - start));
			hStatus->Text = "Downloading Done in " + duration.count() + " milliseconds.";
		}
		else
		{
			for (int i = 0; i < msg->Length; i++)
				hFsListBox->Items->Add(msg[i]);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			typedef std::chrono::duration<int, std::milli> millisecs_t; millisecs_t duration(std::chrono::duration_cast<millisecs_t>(end - start));
			hStatus->Text = "Searching Done in " + duration.count() + " milliseconds.";
		}
	}
	else
	{
		hStatus->Text = "File Not Found";
	}
}


//recv thread recieves message and then invokes add text using action delegates
void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function
	while (true)
	{
			std::string msg = pClient_->getMessage();
			//std::cout << "\n  receive thread calling getMessage()" << msg<<"\n";
			if (msg.find("|") != std::string::npos)
			{
				
				std::vector<std::string> elements;
				std::stringstream ss(msg);
				std::string item;
				while (std::getline(ss, item, '|')) 
				{
					elements.push_back(item);
			    }
				array<String^>^ args = gcnew array<String^>(elements.size() - 1);
				for (size_t i = 1; i < elements.size(); i++)
				{
					args[i - 1] = toSystemString("localhost:"+ elements[0]+ "-" + elements[i]);
				}
				array<Object^>^ parms = gcnew array<Object^> { args };
				Action<array<String^>^>^ act = gcnew Action<array<String^>^>(this, &WPFCppCliDemo::addText);
				Dispatcher->Invoke(act, parms);  // must call addText on main UI thread
			}
			else
			{
				String^ sMsg = toSystemString(msg);
				array<String^>^ argsString = gcnew array<String^>(1);
				argsString[0] = sMsg;
				array<Object^>^ parmsString = gcnew array < Object^ > { argsString };
				Action<array<String^>^>^ act = gcnew Action<array<String^>^>(this, &WPFCppCliDemo::addText);
				Dispatcher->Invoke(act, parmsString);  // must call addText on main UI thread
			}
		}
	}


//Clears the Listbox of Application
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  cleared message text \n");
  hStatus->Text = "Cleared message";
  hUpListBox->Items->Clear();
}

//Clears the Listbox of Application
void WPFCppCliDemo::clearFile(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text \n");
	hStatus->Text = "Cleared message";
	hFsListBox->Items->Clear();
}


//function called when browse button is pressed
//when user selects OK from dialog box,then it automatically uploads the file
void WPFCppCliDemo::browseForFiles(Object^ sender, RoutedEventArgs^ args)
{
	start = std::chrono::steady_clock::now();
	std::cout << "\n  Browsing for files";
	System::Windows::Forms::DialogResult result;
	result = hFileBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		//taking values from checkboxes and then calling Channel Post message function
		selectedFile = hFileBrowserDialog->FileName;
		std::string command = "UPLOAD";
		String ^pathServer1 = hComboBoxServer1->SelectedValue->ToString();
		String ^pathServer2 = hComboBoxServer2->SelectedValue->ToString();
		System::Nullable<bool>^ checkBox1 = checkBoxServer1->IsChecked;
		System::Nullable<bool>^ checkBox2 = checkBoxServer2->IsChecked;
		if (checkBox1->Value)
		{
			pClient_->postMessage(command, toStdString(selectedFile), ".\\Server1\\RootServer1\\" + toStdString(pathServer1), "", server1Port);
			hStatus->Text = "Uploading File .....";
		}
		if (checkBox2->Value)
		{
			pClient_->postMessage(command, toStdString(selectedFile), ".\\Server2\\RootServer2\\" + toStdString(pathServer2), "", server2Port);
			hStatus->Text = "Uploading File .....";
		}
		if (!(checkBox1->Value) && !(checkBox2->Value))
		{
			hStatus->Text = "Please select atleast one server";
		}
	}
}


//function called when user presses download button
void WPFCppCliDemo::callDownloadFile(Object^ sender, RoutedEventArgs^ args)
{
	std::string command = "DOWNLOAD";
	if (hFsListBox->SelectedValue)
	{
		//takes port from listbox and then places it into post message function of Channel
		start = std::chrono::steady_clock::now();
		String^ valueUpload = hFsListBox->SelectedValue->ToString();
		std::string path = toStdString(valueUpload);
		std::string port = path.substr(10, path.find_first_of("-") - 10);
		path = path.substr(path.find_first_of("-") + 1, path.size());
		pClient_->postMessage(command, ".\\Client1\\ReceivedFiles\\", path, "", stoi(port));
		hStatus->Text = "Downloading File .....";
	}
	else
	{
		hStatus->Text = "Please Select alteast one file for downloading";
	}
}

//Function called when filesearch button is pressed
void WPFCppCliDemo::getParameters(Object^ sender, RoutedEventArgs^ args)
{
	clearFile(sender, args);
	start = std::chrono::steady_clock::now();
	//takes values from GUI and then posts into channel post method
	String ^pattern = hTextBoxFileUpPatt->Text;
	String ^pathServer1 =  comboBoxServer1->SelectedValue->ToString();
	String ^pathServer2 = comboBoxServer2->SelectedValue->ToString();
	System::Nullable<bool>^ checkBox1 = fsCheckBoxServer1->IsChecked;
	System::Nullable<bool>^ checkBox2 = fsCheckBoxServer2->IsChecked;
	std::string command = "FINDFILES";
	if (checkBox1->Value)
	{
		pClient_->postMessage(command, toStdString(pattern), ".\\Server1\\RootServer1\\" + toStdString(pathServer1), "", server1Port);
		hStatus->Text = "Searching for Files in Server .....";
	}
	if (checkBox2->Value)
	{
		pClient_->postMessage(command, toStdString(pattern), ".\\Server2\\RootServer2\\" + toStdString(pathServer2), "", server2Port);
		hStatus->Text = "Searching for Files in Server .....";
	}
	if (!(checkBox1->Value) && !(checkBox2->Value))
	{
		hStatus->Text = "Please select atleast one server";
	}
	
}

//Function called when textSearch button is pressed
void WPFCppCliDemo::getParametersTextsearch(Object^ sender, RoutedEventArgs^ args)
{
	clearFile(sender, args);
	start = std::chrono::steady_clock::now();
	//takes values from GUI and then posts into channel post method
	String ^pattern = hTextBoxFileUpPatt->Text;
	String ^pathServer1 = comboBoxServer1->SelectedValue->ToString();
	String ^pathServer2 = comboBoxServer2->SelectedValue->ToString();
	String ^textToBeSearched = hTextBoxFileUpSearch->Text;
	System::Nullable<bool>^ checkBox1 = fsCheckBoxServer1->IsChecked;
	System::Nullable<bool>^ checkBox2 = fsCheckBoxServer2->IsChecked;
	std::string command = "SEARCHTEXT";
	if (toStdString(textToBeSearched) != "")
	{
		if (checkBox1->Value)
		{
			pClient_->postMessage(command, toStdString(pattern), ".\\Server1\\RootServer1\\" + toStdString(pathServer1), toStdString(textToBeSearched), server1Port);
			hStatus->Text = "Searching for text in files present at Server .....";
		}
		if (checkBox2->Value)
		{
			pClient_->postMessage(command, toStdString(pattern), ".\\Server2\\RootServer2\\" + toStdString(pathServer2), toStdString(textToBeSearched), server2Port);
			hStatus->Text = "Searching for text in files present at Server .....";
		}
	}
	if (!(checkBox1->Value) && !(checkBox2->Value))
	{
		hStatus->Text = "Please select atleast one server";
	}
	else if (toStdString(textToBeSearched) == "")
	{
		hStatus->Text = "Please enter Text";
	}
}


void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
}

[STAThread]
int main(array<System::String^>^ args)
{
	if (args->Length >= 3)
	{
		Console::WriteLine(L"\n Starting WPFCppCliDemo");
		Application^ app = gcnew Application();
		app->Run(gcnew WPFCppCliDemo(args));
		Console::WriteLine(L"\n\n");
	}
	else
	{
		Console::WriteLine(L"\n Cannot Start Application. Please Provide Correct Arguments.");
		Console::WriteLine(L"\n See READ ME file for more info");
	}
}