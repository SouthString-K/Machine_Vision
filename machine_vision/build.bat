call "D:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
cd /d D:\code\machine_vision
cl /nologo /EHsc /std:c++17 /MD /I D:\Opencv\opencv\build\include main.cpp /Fe:machine_vision.exe /link /LIBPATH:D:\Opencv\opencv\build\x64\vc16\lib opencv_world4130.lib
set PATH=D:\Opencv\opencv\build\x64\vc16\bin;%PATH%
machine_vision.exe paperclips.jpg nogui
