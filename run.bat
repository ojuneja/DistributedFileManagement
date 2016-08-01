@echo off

echo ***********************************************************************
echo.
echo REMOTE CODE MANAGEMENT
echo. 
echo ***********************************************************************

start "Client1" Debug\CppCli-WPF-App.exe 8087 8089 9000
start "Server1" Debug\Server1.exe 8089
start "Server2" Debug\Server2.exe 9000


