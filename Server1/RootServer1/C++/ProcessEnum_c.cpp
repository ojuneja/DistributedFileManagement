/////////////////////////////////////////////////////////////////////
//  ProcessEnum_c.cpp - Enumerates the processes in a Windows      //
//                      system                                     //
//  ver 1.0                                                        //
//                                                                 //
//  Lanaguage:    Visual C++, Visual Studio 2008                   //
//  Platform:     WinXP                                            //
//  Application:  Help For Project 3, Spring 2009                  //
//  Author:       Phil Pratt-Szeliga, CST 4-116,                   //
//                Syracuse University, (315) 235-5584,             //
//                phil.pratt.szeliga@gmail.com                     //
/////////////////////////////////////////////////////////////////////

#include "ProcessEnum_c.h"

char ProcessEnum_c::wide_to_narrow(WCHAR c)
{
	return (char) c;
}

//Get the current running processes in the system
std::vector<std::pair<std::string, int>> ProcessEnum_c::GetProcesses()
{
	std::vector<std::pair<std::string, int>>  output;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;

	if(!hSnapshot)
	{
		return output;
	}

	// Initialize size in structure
	pe.dwSize = sizeof(pe);
	for(int i = Process32First(hSnapshot, &pe); i; i = Process32Next(hSnapshot, &pe))  //for each process
	{
		std::string curr_process = "";
		for(int i = 0; i < MAX_PATH; i++)
		{
			//Note this gets only the process exe name, not the full path.
			//For the full path see here: http://msdn.microsoft.com/en-us/library/ms684839(VS.85).aspx
			char c = wide_to_narrow(pe.szExeFile[i]);
			if(c == '\0')
			{
				break;
			}
			curr_process += c;
		}
		std::pair<std::string, int> new_entry;
		new_entry.first = curr_process;
		new_entry.second = pe.th32ProcessID;
		output.push_back(new_entry);
	}

	CloseHandle(hSnapshot);

	return output;
}

#ifdef TEST_PROCESS_ENUM_C

int main(int argc, char *argv[])
{
	std::vector<std::pair<std::string, int>> processes = ProcessEnum_c::GetProcesses();

	for(size_t i = 0; i < processes.size(); i++)
	{
		std::cout << processes[i].first << " " << processes[i].second << std::endl;
	}

	return 0;
}

#endif //TEST_PROCESS_ENUM_C