@echo off
setlocal enableextensions

set PROJDIR=%CD%
set SRC=%PROJDIR%\src
set VENDOR=%PROJDIR%\vendor
set SDL=%VENDOR%\SDL2-2.0.5
set SDLLIB=%SDL%\lib\x64

set INCLUDES=-I%SDL%\include -I%VENDOR%\glad\include -I%SRC% -I%VENDOR%\glad\src
set IMPORTS=%SDLLIB%\SDL2.lib %SDLLIB%\SDL2main.lib opengl32.lib
set CXXFLAGS=-O2 -nologo -GR- -EHs- -Zi -WX -W4 -wd4100 -wd4505 -wd4996 -wd4244 -wd4201

if not exist build (mkdir build)
pushd build

xcopy /Q /Y %SDLLIB%\SDL2.dll
xcopy /Q /Y /T /E %PROJDIR%\assets\ assets\
xcopy /Q /Y /E %PROJDIR%\assets\* assets\

cl %CXXFLAGS% %INCLUDES% -c %SRC%\bezier_unity.cpp ^
   -Fd:bezier_unity.pdb -Fo:bezier_unity.obj ^
   -MT -link %IMPORTS% -opt:REF

cl %CXXFLAGS% %INCLUDES% %SRC%\main.cpp ^
   -Fd:bezier.pdb -Fe:bezier.exe bezier_unity.obj ^
   -MT -link %IMPORTS% -opt:REF -subsystem:WINDOWS

popd
