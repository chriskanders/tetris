@echo off
IF NOT EXIST bin	mkdir bin
pushd bin
cl -Zi -FC -W3 -WX -wd4577 -I"C:\dev\stb"^
 -I"C:\dev\glm"^
 -I"C:\dev\glfw\include"^
 -I"C:\dev\glew\include"^
 ..\tetris.cpp^
 -link opengl32.lib^
 -LIBPATH:"C:\dev\glfw\lib-vc2015" glfw3dll.lib^
 -LIBPATH:"C:\dev\glew\lib\Release\x64" glew32.lib^
 -SUBSYSTEM:CONSOLE
popd bin