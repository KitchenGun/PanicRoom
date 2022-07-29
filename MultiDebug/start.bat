::Author: Jackblue
@echo off

:: Editable Variable
set "PROJECT_NAME=PanicRoom"
set UE_PATH="F:\UE_5.0\Engine\Binaries\Win64\UnrealEditor.exe"
set PROJECT_PATH="F:\SGA_UE4\PanicRoom\PanicRoom.uproject"
:: You can edit arguments here (docs : https://docs.unrealengine.com/latest/INT/Programming/Basics/CommandLineArguments/index.html)
set "SERVER_COMMAND=%UE_PATH% %PROJECT_PATH% -server -log"
set "CLIENT_COMMAND=%UE_PATH% %PROJECT_PATH% -game -log"
:: Script
Title Starting %PROJECT_NAME%..


set /p LaunchMode="Launch mode [all(default)/server/client]:"
if "%LaunchMode%" == "server" goto :launchServer
if "%LaunchMode%" == "client" goto :launchClient
if  NOT "%LaunchMode%" == "all" if NOT "%LaunchMode%"=="" exit 
goto :launchAll

:launchServer
start "%PROJECT_NAME% - Dedicated Server" %SERVER_COMMAND%
exit

:launchClient
start "%PROJECT_NAME% - Client" %CLIENT_COMMAND%
exit

:launchAll
start "%PROJECT_NAME% - Dedicated Server" %SERVER_COMMAND%
start "%PROJECT_NAME% - Client" %CLIENT_COMMAND%
exit