# DistributedFileManagement
A Remote File Management consists of transferring files to and from the local and remote machines,
exploring a directory subtree on a remote machine, identifying files and searching for specific texts within files.


#####Requrements can be found [HERE] (https://github.com/ojuneja/DistributedFileManagement/blob/master/Requirements.pdf)

##### Running The Project
Project can be instaniated by running [compile.bat](https://github.com/ojuneja/DistributedFileManagement/blob/master/compile.bat) first 
and then [run.bat] (https://github.com/ojuneja/DistributedFileManagement/blob/master/run.bat) in administrator mode

#####Input
- start Server1.exe 8089  - Format : start Server1.exe [Server1Port] 
- start Server2.exe 9000 - Format : start Server2.exe [Server2Port] 
- start CppCli-WPF-App.exe 8087 8089 9000 - Format : start CppCli-WPF-App.exe [Client1Port]<space>[Server1Port]<space>[Server2Port] 

#####Important Note:
1. Server1 has 3 directories inside its root folder,i.e.,C++,Java,PDF
  Server2 has 3 directories inside its root folder,i.e.,C++,Java,Text
2. Client1 has file repository on path : .\\Client1\\SendFiles (you can upload files from this repository)
3. Downloaded files will be received in .\\Client1\\ReceivedFiles
4. 8089 is port on which server 1 will run
5. 9000 is port on which server 2 will run
6. 8087 is port on which user machine will run(localhost will be the IP address).First port for Server1,second for Server2,third for Client1.


