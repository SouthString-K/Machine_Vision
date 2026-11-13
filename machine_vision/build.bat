@echo off
rem Build and run both programs (MSVC + OpenCV 4.13). Run from this folder.
call "D:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
cd /d "%~dp0"
if not exist build mkdir build
for %%t in (paperclips coins) do (
    cl /nologo /EHsc /std:c++17 /MD /I D:\Opencv\opencv\build\include src\%%t.cpp /Fo:build\ /Fe:build\%%t.exe /link /LIBPATH:D:\Opencv\opencv\build\x64\vc16\lib opencv_world4130.lib
)
set PATH=D:\Opencv\opencv\build\x64\vc16\bin;%PATH%
build\paperclips.exe images\paperclips.jpg nogui
build\coins.exe images\coins.jpg nogui
